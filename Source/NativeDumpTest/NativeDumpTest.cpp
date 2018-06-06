// NativeDumpTest.cpp : Defines the entry point for the console application.
//


#include <string>
#include <vector>
#include <list>
#include <memory>
#include <map>
#include <unordered_map>

#include <stdlib.h>

#ifdef _WIN32
  #include <Windows.h>
  #define NO_INLINE __declspec(noinline)
  #if _MSC_VER < 1900
    #define int8_t __int8
    #define int16_t __int16
    #define int32_t __int32
    #define int64_t __int64
    #define uint8_t unsigned __int8
    #define uint16_t unsigned __int16
    #define uint32_t unsigned __int32
    #define uint64_t unsigned __int64
  #endif
#else
  #include <chrono>
  #include <thread>
  #define NO_INLINE
#endif

using namespace std;

#pragma auto_inline(off)

void DefaultTestCase();

enum MyEnum
{
    enumEntry0,
    enumEntry1,
    enumEntry2,
    enumEntry3,
    enumEntry4,
    enumEntry5,
    enumEntry6,
};

struct BuiltinTypesTest
{
    bool b;
    char c1;
    wchar_t c2;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    float f32;
    double f64;
    long double f80;
} builtinTypesTest;

class MyTestClass
{
public:
    enum MyEnumInner
    {
        simple0,
        simple1,
        simple2,
        simple3,
        simple4,
    };

    wstring string1;
    list<wstring> strings;
    wstring stringArray[100];
    vector<string> ansiStrings;
    map<wstring, string> stringMap;
    unordered_map<wstring, string> stringUMap;
    MyEnum enumeration;
    MyEnumInner innerEnumeration;

    static int staticVariable;
} globalVariable;

int MyTestClass::staticVariable = 1212121212;

void(*defaultTestCaseAddress)() = &DefaultTestCase;

template <typename Type>
struct BasicTemplateType
{
    BasicTemplateType()
    {
        value = 42;
        for (int i = 0; i < 10; i++)
        {
            values[i] = (Type)i;
        }
    }

    Type value;
    Type values[10];
};

#pragma optimize("", off)
NO_INLINE void CauseDump()
{
    // Generate the dump
    //throw std::bad_exception();
    int* a = nullptr;
    *a = 4;
}

NO_INLINE void TestDbgEngDll()
{
    const char* testString = "Testing...";
    const wchar_t* testWString = L"Testing...";
#ifdef WIN32
    CONTEXT context;

    GetThreadContext(GetCurrentThread(), &context);
#endif

    CauseDump();
}

NO_INLINE void TestBasicTemplateType()
{
    // Basic type template classes
    BasicTemplateType<float> floatTemplate;
    BasicTemplateType<double> doubleTemplate;
    BasicTemplateType<int> intTemplate;

    TestDbgEngDll();
}

NO_INLINE void TestSharedWeakPointers()
{
    // Test shared/weak pointers
    shared_ptr<int> sptr1 = make_shared<int>(5);
    weak_ptr<int> wptr1 = sptr1;
    shared_ptr<int> esptr1 = make_shared<int>(42);
    weak_ptr<int> ewptr1 = esptr1;
    shared_ptr<int> esptr2(new int);
    weak_ptr<int> ewptr2 = esptr2;

    esptr1 = nullptr;
    esptr2 = nullptr;

    TestBasicTemplateType();
}

NO_INLINE void TestArray()
{
    int testArray[10000];

    for (int i = 0; i < sizeof(testArray) / sizeof(testArray[0]); i++)
        testArray[i] = 0x12121212;

    TestSharedWeakPointers();
}

NO_INLINE void DefaultTestCase()
{
    MyTestClass * p = &globalVariable;
    MyTestClass ** q = &p;
    MyEnum e = enumEntry3;

    p->string1 = L"qwerty";
    p->strings.push_back(L"Foo");
    p->strings.push_back(L"Bar");
    p->ansiStrings.push_back("AnsiFoo");
    p->ansiStrings.push_back("AnsiBar");
    p->stringMap.insert(make_pair(L"foo", "ansiFoo"));
    p->stringMap.insert(make_pair(L"bar", "ansiBar"));
    p->stringUMap.insert(make_pair(L"foo", "ansiFoo"));
    p->stringUMap.insert(make_pair(L"bar", "ansiBar"));
    p->enumeration = enumEntry2;
    p->innerEnumeration = MyTestClass::simple4;

    TestArray();
}

NO_INLINE void InfiniteRecursionTestCase(int arg)
{
#ifdef _WIN32
    Sleep(100);
#else
    this_thread::sleep_for(chrono::milliseconds(100));
#endif
    InfiniteRecursionTestCase(arg + 1);
}

NO_INLINE int main(int argc, char** argv)
{
    int testCaseToRun = 0;

    if (argc == 2)
    {
        testCaseToRun = atoi(argv[1]);
    }

    printf("%d\n", argc);
    printf("%s\n", argv[0]);
    printf("%d\n", MyTestClass::staticVariable);
    switch (testCaseToRun)
    {
    case 0 :
        defaultTestCaseAddress();
        break;
    case 1:
        InfiniteRecursionTestCase(0);
        break;
    default:
        defaultTestCaseAddress();
    }

    return 0;
}
#pragma optimize("", on)

struct DoubleTest
{
    DoubleTest()
        : d(3.5)
        , f(2.5)
        , i(5)
    {
    }

    double d;
    float f;
    int i;
} doubleTest;


// SimpleMultiClassInheritance
//   struct A
//     char a
//   struct B
//     double b
//   struct C : A : B
//     float c

struct SimpleMultiClassInheritanceA
{
    SimpleMultiClassInheritanceA()
        : a(42)
    {
    }

    char a;
};

struct SimpleMultiClassInheritanceB
{
    SimpleMultiClassInheritanceB()
        : b(42)
    {
    }

    double b;
};

struct SimpleMultiClassInheritanceC : public SimpleMultiClassInheritanceA, public SimpleMultiClassInheritanceB
{
    SimpleMultiClassInheritanceC()
        : c(42)
    {
    }

    float c;
} simpleMultiClassInheritanceTest;


// MultiClassInheritance
//   struct A
//     float GetA()
//     float a
//   struct B
//     int GetB()
//     int b
//   struct C : A, B
//     float GetA()
//     double c
//   struct D : A, B
//     float GetA()
//     int GetB()
//     double d

struct MultiClassInheritanceA
{
    MultiClassInheritanceA()
        : a(42)
    {
    }

    virtual float GetA() const = 0;

    float a;
};

struct MultiClassInheritanceB
{
    MultiClassInheritanceB()
        : b(42)
    {
    }

    virtual int GetB() const
    {
        return b;
    }

    int b;
};

struct MultiClassInheritanceC : public MultiClassInheritanceA, public MultiClassInheritanceB
{
    MultiClassInheritanceC()
        : c(42)
    {
    }

    virtual float GetA() const override
    {
        return a;
    }

    double c;
} multiClassInheritanceTest;

struct MultiClassInheritanceD : public MultiClassInheritanceA, public MultiClassInheritanceB
{
    MultiClassInheritanceD()
        : d(42)
    {
    }

    virtual float GetA() const override
    {
        return a;
    }

    virtual int GetB() const override
    {
        return b;
    }

    double d;
} multiClassInheritanceTest2;


// VirtualMultiClassInheritance
//   struct A
//     char a
//   struct B : virtual A
//     double b
//   struct C : virtual A
//     float c
//   struct D : B, C
//     int d
//   struct E : virtual A, B, C
//     int e

struct VirtualMultiClassInheritanceA
{
    VirtualMultiClassInheritanceA()
        : a(42)
    {
    }

    char a;
};

struct VirtualMultiClassInheritanceB : public virtual VirtualMultiClassInheritanceA
{
    VirtualMultiClassInheritanceB()
        : b(42)
    {
    }

    double b;
};

struct VirtualMultiClassInheritanceC : public virtual VirtualMultiClassInheritanceA
{
    VirtualMultiClassInheritanceC()
        : c(42)
    {
    }

    float c;
} VirtualMultiClassInheritanceTest;

struct VirtualMultiClassInheritanceD : public VirtualMultiClassInheritanceB, public VirtualMultiClassInheritanceC
{
    VirtualMultiClassInheritanceD()
        : d(42)
    {
    }

    int d;
} virtualMultiClassInheritanceTest;

struct VirtualMultiClassInheritanceE
    : public virtual VirtualMultiClassInheritanceA
    , public VirtualMultiClassInheritanceC
    , public VirtualMultiClassInheritanceB
{
    VirtualMultiClassInheritanceE()
        : e(42)
    {
    }

    int e;
} virtualMultiClassInheritanceTest2;
