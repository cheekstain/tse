// preprocessing macro for unit test
// taken from cs50 lecture
// each test should start by setting the result count to zero
#define START_TEST_CASE(name) int _failures=0; char *_testname = (name);

// Check a condition; if false, print warning message.
// e.g., EXPECT(dict->start == NULL).
// note: the preprocessor 
//   converts __LINE__ into line number where this macro was used, and
//   converts "#x" into a string constant for arg x.
#define EXPECT(x)                                               \
  if (!(x)) {                                                   \
    _failures++;                                                \
    printf("Fail %s Line %d: [%s]\n", _testname, __LINE__, #x); \
  }

// return the result count at the end of a test
#define END_TEST_CASE                                                   \
  if (_failures == 0) {                                                 \
    printf("PASS test %s\n\n", _testname);                              \
  } else {                                                              \
    printf("FAIL test %s with %d errors\n\n", _testname, _failures);    \
  }

#define TEST_RESULT (_failures)
