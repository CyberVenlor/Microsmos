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
    //UtilityFunctions::print(continuous_collision_detection());
    //for_each_thread([delta](PhysicsObject& obj) { obj.solve_spring(delta); });
    for_each_thread([delta](PhysicsObject& obj) { obj.solve_spring(delta); });
    for_each_thread([delta](PhysicsObject& obj) { obj.solve_edge(delta); });
    for_each_thread([delta](PhysicsObject& obj) { obj.solve_point(delta); });
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
        GodotAsseccer::get_singleton()->draw_circle(point.position, 2, Color(0,0,0,1));
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

void PhysicsObject::solve_force(f32 delta){
    for (auto& point : points){
        point.position += point.velocity * delta;

        point.velocity += Vector2(0.001, 0.3);

        if (point.position.y > 200){
            point.velocity.y *= -1;
            point.position.y = 200;
        }
    }
}

void PhysicsObject::solve_point(f32 delta){
    for (auto& point : points){
        point.position += point.velocity * delta;

        point.velocity += Vector2(0.001, 0.3);

        if (point.position.y > 200){
            point.velocity.y *= -1;
            point.position.y = 200;
        }
    }
}

void PhysicsObject::solve_spring(f32 delta){
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

void PhysicsObject::solve_edge(f32 delta){
    for (auto& edge :edges){
        for (auto& physics_object : physics_objects){
            for (auto& p : physics_object.points){
                Point& p0 = points[edge.p0];
                Point& p1 = points[edge.p1];

                f32 t = continuous_collision_detection(p, p0, p1, delta);

                if (t != f32Inf){
                    // p0.position += p0.velocity * t;
                    // p1.position += p1.velocity * t;
                    // p.position += p.velocity * t;

                    // 计算线段的方向向量和法向量
                    Vector2 line_direction = (p1.position - p0.position).normalized();
                    Vector2 normal = Vector2(-line_direction.y, line_direction.x); // 线段正面的法线

                    // 判断法线方向是否正确，基于质点位置相对于线段的位置
                    if ((p.position - p0.position).dot(normal) < 0) {
                        normal = -normal; // 如果质点在线段背面，反转法线方向
                        
                    }
                    normal = -normal;

                    const f32 e = 2;

                    float v_n = p.velocity.dot(normal);
                    float v0_n = p0.velocity.dot(normal);
                    float v1_n = p1.velocity.dot(normal);

                    // 计算线段中点的速度
                    float m0 = p0.mass;
                    float m1 = p1.mass;
                    float m_line = m0 + m1;

                    float v_line = (m0 * v0_n + m1 * v1_n) / m_line;

                    // 计算弹性碰撞后的目标中点速度
                    float v_line_prime = v_line + e * (v_line - v_n);

                    // 计算速度变化量
                    float delta_v_n = m_line * (v_line_prime - v_line) / (p.mass + m_line);

                    

                    // 更新质点 p 的速度
                    p.velocity += delta_v_n * normal;

                    // 更新质点 p0 和 p1 的速度
                    float delta_v0_n = (p0.mass * delta_v_n) / m_line;
                    p0.velocity += -delta_v0_n * normal;

                    float delta_v1_n = (p1.mass * delta_v_n) / m_line;
                    p1.velocity += -delta_v1_n * normal;

                    UtilityFunctions::print(delta_v_n * normal);
                    UtilityFunctions::print(-delta_v0_n * normal);
                    UtilityFunctions::print(-delta_v1_n * normal);
                }
            }
        }
    }
}

f32 PhysicsObject::continuous_collision_detection(const Point& p, const Point& p0, const Point& p1, f32 delta) {
    // 计算系数 A, B, C
    const f32 A = (p.velocity.x - p0.velocity.x) * (p.velocity.y - p1.velocity.y)
                - (p.velocity.y - p0.velocity.y) * (p.velocity.x - p1.velocity.x);

    const f32 B = (p.position.x - p0.position.x) * (p.velocity.y - p1.velocity.y)
                + (p.velocity.x - p0.velocity.x) * (p.position.y - p1.position.y)
                - (p.position.y - p0.position.y) * (p.velocity.x - p1.velocity.x)
                - (p.velocity.y - p0.velocity.y) * (p.position.x - p1.position.x);

    const f32 C = (p.position.x - p0.position.x) * (p.position.y - p1.position.y)
                - (p.position.y - p0.position.y) * (p.position.x - p1.position.x);

    auto const is_on_line = [](f32 Px, f32 Ux, f32 Vx) {
        return (Px > Ux && Px < Vx) || (Px > Vx && Px < Ux);
    };

    // 判断 A 是否接近 0 (共线或平行)
    if ( A == 0) {
        if ( B != 0) {
            f32 t = -C / B;
            if (t > 0 && t <= delta && is_on_line(p.position.x + p.velocity.x * t, p0.position.x + p0.velocity.x * t, p1.position.x + p1.velocity.x * t)) {
                return t;
            }
        }
        return f32Inf; // 无解
    }

    const f32 discriminant = B * B - 4 * A * C;
    if (discriminant < 0) {
        return f32Inf; // 无碰撞
    }

    const f32 sqrt_discriminant = sqrt(discriminant);
    const f32 t1 = (-B - sqrt_discriminant) / (2 * A);
    const f32 t2 = (-B + sqrt_discriminant) / (2 * A);

    // 找到有效解并返回最小的
    const bool t1_valid = (t1 > 0 && t1 <= delta && is_on_line(p.position.x + p.velocity.x * t1, p0.position.x + p0.velocity.x * t1, p1.position.x + p1.velocity.x * t1));
    const bool t2_valid = (t2 > 0 && t2 <= delta && is_on_line(p.position.x + p.velocity.x * t2, p0.position.x + p0.velocity.x * t2, p1.position.x + p1.velocity.x * t2));

    if (t1_valid && t2_valid) return std::min(t1, t2);
    if (t1_valid) return t1;
    if (t2_valid) return t2;

    return f32Inf; // 没有碰撞
}