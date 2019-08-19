//
//  NativeThreadLocal.cpp
//  NativeThreadLocal
//
//  Created by Silas on 2018/5/22.
//  Copyright © 2018年 Silas. All rights reserved.
//

#include <vector>
#include <thread>

typedef void (*FuncDisposeObj)(void* obj);
struct ThrealLocalContainer
{
public:
    ThrealLocalContainer()
    {
        funcDispose = nullptr;
        innerobj = nullptr;
    }
    ThrealLocalContainer(void* obj, FuncDisposeObj func_dispose)
    {
        funcDispose = func_dispose;
        innerobj = obj;
    }
    ~ThrealLocalContainer()
    {
        if (funcDispose && innerobj)
        {
            funcDispose(innerobj);
        }
    }
    ThrealLocalContainer(ThrealLocalContainer&& other)
    {
        funcDispose = other.funcDispose;
        innerobj = other.innerobj;
        other.funcDispose = nullptr;
        other.innerobj = nullptr;
    }
    ThrealLocalContainer(ThrealLocalContainer&) = delete;

    void SetContainer(void* obj, FuncDisposeObj func_dispose)
    {
        this->~ThrealLocalContainer();
        funcDispose = func_dispose;
        innerobj = obj;
    }
    void* GetContainer()
    {
        return innerobj;
    }
private:
    FuncDisposeObj funcDispose;
    void* innerobj;
};

static thread_local ThrealLocalContainer container;

extern "C"
{
void RegThreadLocalContainer(void* obj, FuncDisposeObj func_dispose)
{
    container.SetContainer(obj, func_dispose);
}
void* GetThreadLocalContainer()
{
    return container.GetContainer();
}
void* GetThreadID()
{
    auto id = std::this_thread::get_id();
    return (void*)*(pthread_t*)&id;
}
}
