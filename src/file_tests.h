#ifndef FILE_TESTS_H
#define FILE_TESTS_H

double fprintfTest(size_t iterations, char *buff, size_t buff_size);
double buffered_fwriteTest(size_t iterations, char *buff, 
  size_t buff_size);
double binary_fwriteTest(size_t iterations, char *buff, 
  size_t buff_size);
double bufferedBinary_fwriteTest(size_t iterations, char *buff, 
  size_t buff_size);

double ofstreamTest(size_t iterations, char *buff, size_t buff_size);
double buffered_ofstreamWriteTest(size_t iterations, char *buff, 
  size_t buff_size);
double binary_ofstreamWriteTest(size_t iterations, char *buff, 
  size_t buff_size);
double bufferedBinary_ofstreamWriteTest(size_t iterations, char *buff, 
  size_t buff_size);

double vtemplate_fprintfTest(size_t iterations, char *buff, 
  size_t buff_size);
double vtemplate_ofstreamTest(size_t iterations, char *buff, 
  size_t buff_size);

double KameUtil_ofstreamTest(size_t iterations, char *buff, 
  size_t buff_size);
double KameUtil_fprintfTest(size_t iterations, char *buff, 
  size_t buff_size);
double KameUtil_buffered_fwriteTest(size_t iterations, char *buff, 
  size_t buff_size);
double KameUtil_bufferedBinary_fwriteTest(size_t iterations, char *buff, 
  size_t buff_size);

#endif
