#include "TestImagempi.h"
#include "Funcs.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

double m_Tolerance = 1e-15;

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
//	   if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
//		         return -1;
//			    }
//	      else {
//		            return 0;
//			       }

}

/* The suite cleanup function.
    * Closes the temporary file used by the tests.
     * Returns zero on success, non-zero otherwise.
      */
int clean_suite1(void)
{
//	   if (0 != fclose(temp_file)) {
//		         return -1;
//			    }
//	      else {
//		            temp_file = NULL;
//			          return 0;
//				     }
}

void testFindnbrs(void)
{

  /* Insert your assertion functions here. Tip: try CU_ASSERT_DOUBLE_EQUAL f.e. */

    //CU_FAIL("No test implemented\n");
	int comm = 1;
	int 
	CU_ASSERT_TRUE()
}

void testNegativeBoundary(void)
{

  /* Test how program behaves when a negative number is passed on input.
     Tip: return an error code if the input is invalid */

     //CU_FAIL("No test implemented\n");
     
}

void testOver360Boundary(void)
{

  //CU_FAIL("No test implemented\n");
  
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 * No need to modify the main function
 * For more information visit CUNIT page at sourceforge.org
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", NULL, NULL);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test correctness", testCorrectness))) // ||
    //   (NULL == CU_add_test(pSuite, "test negative boundary", testNegativeBoundary)) ||       
    //   (NULL == CU_add_test(pSuite, "test over 360 boundary", testOver360Boundary)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE); 
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

