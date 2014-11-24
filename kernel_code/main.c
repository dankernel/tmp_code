/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014년 08월 05일 03시 41분 16초
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         author:  Jun-Hyung Park (), google@dankook.ac.kr
 *   organization:  Dankook Univ.
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define ERR_ARG_NULL      0x00000001
#define ERR_ARG_NEGATIVE  0x00000002

int print_over_word(char *buf, int i, int len)
{
  char *print_buf = NULL;

  if (!buf)
    return -ERR_ARG_NULL;

  /* alloc tmp mem */
  print_buf = malloc(sizeof(char) * len + 1);
  memset(print_buf, '\0', len + 1);
  
  /* copy string */
  strncpy(print_buf, buf + i - len, len);
  printf("[OVER] len : %d \n", len);
  printf("%s \n", print_buf);

  /* free */
  free(print_buf);

  return 0;
}

int line_word_size_chaeek(char *buf, int file_size, int limit_size)
{
  int i = 0;
  int tmp_word_count = 0;

  if (!buf)
    return -ERR_ARG_NULL;

  if (file_size < 0 || limit_size < 0)
    return -ERR_ARG_NEGATIVE;

  /* cheek */
  while (i < file_size) {

    tmp_word_count = 0;
    while (buf[i] != '\n') {
      tmp_word_count++;
      i++;
    }

    if (limit_size < tmp_word_count)
      print_over_word(buf, i, tmp_word_count);

    /* next char */
    i++;

  }

  return 0;
}

/*  
 * return fd
 */
int open_file(char *path)
{
  int fd = 0;

  if (!path)
    return -ERR_ARG_NULL;

  if ((fd = open(path, O_RDONLY)) < 0)
    return -1;
  else {
    printf("[OK] open..\n");
    return fd;
  }

}

int read_file(char *path, int fd, char **ret_buf)
{
  int file_size = 0;
  int ret = 0;
  struct stat file_info;
  char *buf = NULL;

  /* get file size */
  stat(path, &file_info);
  file_size = file_info.st_size;
  printf("file size : %d \n", file_size);

  /* init buf */
  buf = malloc(sizeof(char) * file_size);

  /* read file and save buf */
  if ((ret = read(fd, buf, file_size)) < 0)
    return -1;
  else
    printf("[OK] read.. \n");

  *ret_buf = buf;

  return file_size;

}

int close_file(int fd, char *buf)
{
  close(fd);
  free(buf);

  return 0;
}

char *next_line(char **str)
{
  int len = 0;
  char *ret = NULL;

  if (!*str)
    return NULL;

  while (*(*str + len++) != '\n');

  ret = malloc(sizeof(char) * len + 1);
  strncpy(ret, *str, len);

  *str += len;

  return ret;
}

int main(int argc, char* argv[])
{
  int fd_code = 0;
  int fd_list = 0;
  int size = 0;
  char *buf = NULL;
  char *file_list = NULL;

  char *file_path = NULL;
  int i = 0;


  /* get list */
  fd_list = open_file("./kernel_file_list");
  read_file("./kernel_file_list", fd_list, &file_list);

  file_path = malloc(sizeof(char) * 1024);
  memset(file_path, '\0', 1024);
  file_path = next_line(&file_list);
  printf("next : %s \n", file_path);
  file_path = next_line(&file_list);
  printf("next : %s \n", file_path);


  /*
  while (*(file_path) != EOF) {

    memset(file_path, '\0', 1024);
    file_path = next_line(file_list);
    printf("next : %s \n", file_path);

    fd_code = open_file(file_path);
    size = read_file(file_path, fd_code, &buf);
    line_word_size_chaeek(buf, size, 30);
    close_file(fd_code, buf);

  }
  */

  return 0;
}

