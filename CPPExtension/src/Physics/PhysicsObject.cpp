#include "PhysicsObject.h"
#include "GodotAsseccer.h"
#include <thread>

Pool<PhysicsObject> PhysicsObject::physics_objects;

PhysicsObject::PhysicsObject(const Ref<BodyData>& body_data, const Vector2& position){
    if (body_data.is_null()) return;
    points = body_data->get_points();
    edges = body_data->get_edges();
    springs = body_data->get_springs();
    //point前处理
    for (auto& point : points){
        point.position += position;
    }
    //spring前处理
    for (auto& spring : springs){
        spring.length = points[spring.p0].position.distance_to(points[spring.p1].position);
    }
}

void PhysicsObject::process(float delta) {
    for_each([delta](PhysicsObject& obj) { obj.solve_spring(delta); });
    for_each([delta](PhysicsObject& obj) { obj.solve_point(delta); });
}

void PhysicsObject::draw(){
    for_each([](PhysicsObject& obj) { obj.draw_point(); });
    for_each([](PhysicsObject& obj) { obj.draw_spring(); });
    for_each([](PhysicsObject& obj) { obj.draw_edge(); });
}

void PhysicsObject::for_each_thread(const std::function<void(PhysicsObject&)>& func) {
    u32 thread_count = thread::hardware_concurrency();
    if (thread_count == 0) {
        thread_count = 2; // 默认线程数
    }

    vector<thread> threads;
    size_t object_count = physics_objects.size();
    size_t chunk_size = (object_count + thread_count - 1) / thread_count; // 向上取整的分块大小

    for (u32 i = 0; i < thread_count; ++i) {
        size_t start_index = i * chunk_size;
        size_t end_index = std::min(start_index + chunk_size, object_count);
        threads.push_back(thread(
            [start_index, end_index, &func](){
                for (size_t j = start_index; j < end_index; j++) {
                    func(physics_objects[j]);
                }
            }
        ));
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }
}

void PhysicsObject::for_each(const std::function<void(PhysicsObject&)>& func){
    for (auto& physics_object : physics_objects){
        func(physics_object);
    }
}

void PhysicsObject::draw_point(){
    for (auto& point : points) {
        GodotAsseccer::get_singleton()->draw_circle(point.position, 2, point.is_collided ? Color(1,1,1,1) : Color(0,0,0,1));
    }
}

void PhysicsObject::draw_edge(){
    for (auto& edge : edges){
        GodotAsseccer::get_singleton()->draw_line(points[edge.p0].position, points[edge.p1].position, Color(0,1,0,1));
    }
}

void PhysicsObject::draw_spring(){
    for (auto& spring : springs){
        GodotAsseccer::get_singleton()->draw_line(points[spring.p0].position, points[spring.p1].position, Color(0,0,1,1));
    }
}

void PhysicsObject::solve_point(float delta){
    UtilityFunctions::print("solve");
    for (u32 p = 0; p < points.size(); p++){
        Point& point = points[p];
        //UtilityFunctions::print("point");
        for (auto& physics_object : physics_objects){
            for (auto& edge : physics_object.edges){
                f32 t = continuous_collision_detection(
                    point.position, point.velocity,
                    physics_object.points[edge.p0].position, physics_object.points[edge.p0].velocity,
                    physics_object.points[edge.p1].position, physics_object.points[edge.p1].velocity,
                    delta
                );
                //UtilityFunctions::print("edge");
                if (t != std::numeric_limits<f32>::infinity()){
                    UtilityFunctions::print(t);
                    //point.position += point.velocity * t;
                    point.velocity *= -1;
                    //physics_object.points[edge.p0].position += physics_object.points[edge.p0].velocity * t;
                    // physics_object.points[edge.p0].velocity *= -0.5;
                    // //physics_object.points[edge.p1].position += physics_object.points[edge.p1].velocity * t;
                    // physics_object.points[edge.p1].velocity *= -0.5;
                }
            }
        }
        
        
    }

    for (auto& point : points){
        point.position += point.velocity * delta;

        point.velocity += Vector2(0.001, 0.1);

        if (point.position.y > 200){
            point.velocity.y *= -1;
            point.position.y = 200;
        }
    }
}

void PhysicsObject::solve_spring(float delta){
    for (auto& spring : springs){
        Point& p0 = points[spring.p0];
        Point& p1 = points[spring.p1];

        Vector2 force = (p1.position - p0.position).normalized()
				* (p0.position.distance_to(p1.position) - spring.length) 
				* spring.strength
				+ (p1.velocity - p0.velocity) * spring.damping;//阻尼

        p0.velocity += force * delta / p0.mass;
		p1.velocity -= force * delta / p1.mass;
    }
}

f32 PhysicsObject::continuous_collision_detection(Vector2 Pp, Vector2 Vp, Vector2 Pu, Vector2 Vu, Vector2 Pv, Vector2 Vv, f32 delta) {
    // 计算系数 A, B, C
    f32 A = (Vp.x - Vu.x) * (Vp.y - Vv.y)
          - (Vp.y - Vu.y) * (Vp.x - Vv.x);

    f32 B = (Pp.x - Pu.x) * (Vp.y - Vv.y)
          + (Vp.x - Vu.x) * (Pp.y - Pv.y)
          - (Pp.y - Pu.y) * (Vp.x - Vv.x)
          - (Vp.y - Vu.y) * (Pp.x - Pv.x);

    f32 C = (Pp.x - Pu.x) * (Pp.y - Pv.y)
          - (Pp.y - Pu.y) * (Pp.x - Pv.x);

    auto is_on_line = [](f32 Px, f32 Ux, f32 Vx){
        //UtilityFunctions::print(Ux, ",", Px, ",", Vx);
        return (Px > Ux && Px < Vx) || (Px > Vx && Px < Ux);
    };

     // 计算判别式
    if (std::abs(A) == 0) {
        if (B != 0) {
            f32 t = -C / B;
            if (t > 0 && t < delta && is_on_line(Pp.x + Vp.x * t, Pu.x + Vu.x * t, Pv.x + Vv.x * t)) {
                return t;
            }
        }
        return std::numeric_limits<f32>::infinity(); // 无解
    }
    
    f32 discriminant = B * B - 4 * A * C;
    if (discriminant < 0) {
        return std::numeric_limits<f32>::infinity(); // 无碰撞
    }

    

    //first solution
    f32 t = (A > 0) ? (-B - sqrt(discriminant)) / (A * 2) : (-B + sqrt(discriminant)) / (A * 2);
    if (t > 0 && t < delta && is_on_line(Pp.x + Vp.x * t, Pu.x + Vu.x * t, Pv.x + Vv.x * t)){
        return t;
    }

    //second solution
    t = (A > 0) ? (-B + sqrt(discriminant)) / (A * 2) : (-B - sqrt(discriminant)) / (A * 2);
    if (t > 0 && t < delta && is_on_line(Pp.x + Vp.x * t, Pu.x + Vu.x * t, Pv.x + Vv.x * t)){
        return t;
    }

    return std::numeric_limits<f32>::infinity();
}
