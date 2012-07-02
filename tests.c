#include <CuTest.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

struct unit;
extern int error_count;
extern int warning_count;
extern void mock_input(const char * input);
extern char *getbuf(void);
extern char *igetstr(char *);
extern void checkgiving(void);
extern void checkanorder(char *);
extern struct unit *newunit(int n, int t);
extern void set_order_unit(struct unit * u);

static void test_nothing(CuTest * tc)
{
  CuAssertTrue(tc, 1);
}

static void test_getbuf(CuTest * tc)
{
  mock_input("herp derp\nherpderp\n");
  CuAssertStrEquals(tc, "herp derp", getbuf());
  CuAssertStrEquals(tc, "herpderp", getbuf());
}

static void test_igetstr(CuTest * tc)
{
  CuAssertStrEquals(tc, "herp", igetstr("herp derp"));
  CuAssertStrEquals(tc, "derp", igetstr(0));
}

static void test_give_each(CuTest * tc)
{
  set_order_unit(newunit(1, 0));
  mock_input("GIB 2 JE 1 SILBER\n");
  error_count = warning_count = 0;
  checkanorder(getbuf());
  CuAssertIntEquals(tc, 0, error_count);
  CuAssertIntEquals(tc, 1, warning_count);
}

int AddTestSuites(CuSuite * suite, const char * args)
{
  char * names = (args && strcmp(args, "all")!=0) ? strdup(args) : strdup("echeck,give");
  char * name = strtok(names, ",");
  CuSuite * cs;

  while (name) {
    if (strcmp(name, "echeck")==0) {
      cs = CuSuiteNew();
      SUITE_ADD_TEST(cs, test_getbuf);
      SUITE_ADD_TEST(cs, test_igetstr);
      SUITE_ADD_TEST(cs, test_nothing);
      CuSuiteAddSuite(suite, cs);
    }
    else if (strcmp(name, "give")==0) {
      cs = CuSuiteNew();
      SUITE_ADD_TEST(cs, test_give_each);
      CuSuiteAddSuite(suite, cs);
    }
    name = strtok(0, ",");
  }
  return 0;
}

int RunAllTests(CuSuite * suite)
{
  CuString *output = CuStringNew();

  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);

  return suite->failCount;
}

