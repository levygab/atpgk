if(EXISTS "/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/Test-ATPGK[1]_tests.cmake")
  include("/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/Test-ATPGK[1]_tests.cmake")
else()
  add_test(Test-ATPGK_NOT_BUILT Test-ATPGK_NOT_BUILT)
endif()
