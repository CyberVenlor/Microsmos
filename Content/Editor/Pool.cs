using System.Collections;
using System.Collections.Generic;
using Godot;

public class Pool<T> : IEnumerable<T> {
    List<(T obj, bool isActive)> pool = new ();
    List<int> freeList = new ();

    public T this[int index]{
        get {
            if (pool[index].isActive == false) GD.PrintErr("try to access inactive item");
            return pool[index].obj;
        }
        set {
            if (pool[index].isActive == true) pool[index] = (value, true);
        }
    }
    
    public int Add(T obj){
        if (freeList.Count == 0){
            pool.Add((obj, true));
            return pool.Count - 1;
        }
        else {
            int index = freeList[freeList.Count - 1];
            pool[index] = (obj, true);
            freeList.RemoveAt(freeList.Count - 1);
            return index;
        }
    }

    public void Remove(int index){
        pool[index] = pool[index] with {isActive = false};
        freeList.Add(index);
    }

    public bool IsActive(int index){
        return pool[index].isActive;
    }

    public IEnumerator<T> GetEnumerator()
    {
        return new CustomEnumerator(this);
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }

    public int Count{
        get => pool.Count;
    }

    private class CustomEnumerator : IEnumerator<T>
    {
        private Pool<T> _customList;
        private int _index;

        public CustomEnumerator(Pool<T> customList)
        {
            _customList = customList;
            _index = -1;
        }

        public T Current => _customList.pool[_index].obj;

        object IEnumerator.Current => Current;

        public bool MoveNext(){//自动跳过inactive项
            while (_index < _customList.Count){
                _index++;
                if (_index < _customList.Count){
                    if (_customList.IsActive(_index)){
                        return true;
                    }
                }
            }
            return false;
        }

        public void Reset()
        {
            _index = -1;
        }

        public void Dispose()
        {
            // 可选：清理资源
        }
    }

}