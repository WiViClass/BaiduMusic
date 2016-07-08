#ifndef __UIDELEGATE_H__
#define __UIDELEGATE_H__

#pragma once

namespace DuiLib {
//委托模式虚基类
//包含：函数的指针、this指针、invoke函数和其他方便使用的重载操作符；
//其实就是保存了一个对象指针和它的一个成员函数的指针，
//invoke函数用于调用对象的成员函数。
class UILIB_API CDelegateBase	 
{
public:
	/*- 构造函数，pObject为委托者的对象， pFn为执行方法*/
    CDelegateBase(void* pObject, void* pFn);
	//拷贝构造函数
    CDelegateBase(const CDelegateBase& rhs);
    virtual ~CDelegateBase();
	/*- 比较两个委托者, true 一致 false不同  */
    bool Equals(const CDelegateBase& rhs) const;
	/*- 响应委托进行处理  */
    bool operator() (void*  );
    virtual CDelegateBase* Copy() const = 0; // add const for gcc

protected:
    void* GetFn();
    void* GetObject();
	 //调用执行方法  
    virtual bool Invoke(void* param) = 0;

private:
    void* m_pObject;
    void* m_pFn;
};
/*静态委托，委托者为空，但有执行过程 */ 
class CDelegateStatic: public CDelegateBase
{
    typedef bool (*Fn)(void*);
public:
    CDelegateStatic(Fn pFn) : CDelegateBase(NULL, pFn) { } 
    CDelegateStatic(const CDelegateStatic& rhs) : CDelegateBase(rhs) { } 
    virtual CDelegateBase* Copy() const { return new CDelegateStatic(*this); }

protected:
    virtual bool Invoke(void* param)
    {
        Fn pFn = (Fn)GetFn();
        return (*pFn)(param); 
    }
};

template <class O, class T>
//委托，委托者不为空  
class CDelegate : public CDelegateBase
{
    typedef bool (T::* Fn)(void*);
public:
    CDelegate(O* pObj, Fn pFn) : CDelegateBase(pObj, &pFn), m_pFn(pFn) { }
    CDelegate(const CDelegate& rhs) : CDelegateBase(rhs) { m_pFn = rhs.m_pFn; } 
    virtual CDelegateBase* Copy() const { return new CDelegate(*this); }

protected:
	//调用执行方法 
    virtual bool Invoke(void* param)
    {
        O* pObject = (O*) GetObject();
        return (pObject->*m_pFn)(param); 
    }  

private:
    Fn m_pFn;
};

template <class O, class T>
CDelegate<O, T> MakeDelegate(O* pObject, bool (T::* pFn)(void*))
{
    return CDelegate<O, T>(pObject, pFn);
}

inline CDelegateStatic MakeDelegate(bool (*pFn)(void*))
{
    return CDelegateStatic(pFn); 
}
//事件源 
//主要成员变量：事件代理指针数组、重载的各个操作符；
//注意：CEventSource的+=操作符，将事件代理CDelegate对象保到其成员变量中；
//在需要的时候可以调用代理中的函数（通过invoke函数），


class UILIB_API CEventSource
{
    typedef bool (*FnType)(void*);
public:
    ~CEventSource();
    operator bool();
    void operator+= (const CDelegateBase& d); // add const for gcc
    void operator+= (FnType pFn);
    void operator-= (const CDelegateBase& d);
    void operator-= (FnType pFn);
    bool operator() (void* param);

protected:
    CStdPtrArray m_aDelegates;
};

} // namespace DuiLib

#endif // __UIDELEGATE_H__