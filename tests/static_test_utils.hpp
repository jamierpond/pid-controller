
#define EXPECT(condition) \
    if (!(condition)) { \
        return false; \
    }


#define STATIC_TEST(my_inline_function) \
    static_assert([] { my_inline_function(); return true; }(), #my_inline_function " failed")
