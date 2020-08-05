#pragma once
#include <iostream>
#include <vector>

class Assert {
    static void checkCondition(bool condition) {
        if(!condition)
            throw false;
    }

public:
    static void isTrue(bool condition) {
        checkCondition(condition);
    }

    template <typename T>
    static bool isEqual(T value1, T value2) {
        checkCondition(value1 == value2);
    }

    template <typename T>
    static bool isNotEqual(T value1, T value2) {
        checkCondition(!(value1 == value2));
    }
};

using TestFunctionPtr = void(*)();

class Test {
public:
    const TestFunctionPtr _function;
    const std::string _name;

    Test(const TestFunctionPtr f, const char* name) : _function(f), _name(name) {}

    bool run() {
        try {
            _function();
        }
        catch (...) {
            return false;
        }
        return true;
    }
};

class TestSuite {
    std::vector<Test> _tests;

public:
    void registerTest(const Test& t) {
        _tests.push_back(t);
    }

    void registerTest(const TestFunctionPtr f, const char* name) {
        registerTest(Test(f,name));
    }

    void run() {
        std::cout << "Running tests...\n\n" << std::flush;

        for(Test& _test : _tests) {
            std::cout
                << "["
                << (_test.run() ? "OK    " : "Failed")
                << "] "
                << _test._name
                << std::endl;
        }
    }
};

inline TestSuite& GetGlobalTestSuite() {
    static TestSuite *tests = new TestSuite;
    return *tests;
}

struct RegisterTest {
    RegisterTest(const TestFunctionPtr f, const char* name) {
        GetGlobalTestSuite().registerTest(f, name);
    }

};



#define TEST(name) void name(); RegisterTest test_##name(name,#name); void name()

#define IS_TRUE(condition) Assert::isTrue(condition);

#define IS_EQUAL(value1,value2) Assert::isEqual(value1,value2);

#define IS_NOT_EQUAL(value1,value2) Assert::isNotEqual(value1,value2);

