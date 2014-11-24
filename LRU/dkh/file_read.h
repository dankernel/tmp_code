/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  file read header
 *
 *        Version:  1.0
 *        Created:  2014년 08월 18일 02시 46분 01초
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

#include <sys/types.h>
#include <unistd.h>

#include "errno.h"
#include "print_msg.h"

#define MAX_BUFF_SIZE 1024 * 1024 * 4

/*
 * file info struct
 */
struct file_info
{/*{{{*/

  char *path;
  int fd;

  int file_size;
  int seek;
  int line;

  int buf_read;

  int buf_size;
  char *buf;
  char *result;

};/*}}}*/

/*
 * init file_info struct
 * @info : file_info struct
 * @path : open target file
 * return : err
 */
int init_file_struct(struct file_info *info, char *path)
{/*{{{*/

  struct stat stat_info;

  if (!info || !path)
    return -EARG_NULL;
  
  /* path */
  info->path = malloc(strlen(path) + 1);
  strcpy(info->path, path);

  /* fd */
  info->fd = open_file(path);

  /* file size */
  stat(path, &stat_info);
  info->file_size = stat_info.st_size;

  /* seek and line */
  info->seek = 0;
  info->line = 0;

  /* buf size */
  if (info->file_size < MAX_BUFF_SIZE)
    info->buf_size = info->file_size;
  else 
    info->buf_size = MAX_BUFF_SIZE;

  /* buffer */
  info->buf_read = 0;
  info->buf = malloc(info->buf_size);
  info->result = malloc(info->buf_size);
  memset(info->buf, '\0', info->buf_size);
  memset(info->result, '\0', info->buf_size);

  return 0;
}/*}}}*/

/*
 * open file
 * @path : file path
 * return : fd
 */
int open_file(char *path)
{/*{{{*/
  int ret = -1;

  if (!path)
    return -EARG_NULL;

  ret = open(path, O_RDWR, 0666);
  if (ret < 0)
    return -EFAIL_FUNC;

ret:
  // err_test(ret, "open");
  return ret;
}/*}}}*/

/*
 * close and free file_info
 * s : struct
 * return : errer code
 */
int close_file_info(struct file_info *s)
{/*{{{*/
  int ret = 0;

  if (!s)
    return -EARG_NULL;

  /* close */
  if (s->fd)
    close(s->fd);

  /* free */
  if (s->path)
    free(s->path);

  if (s->buf)
    free(s->buf);

  if (s->result)
    free(s->result);

  if (s)
    free(s);

  return ret;
}/*}}}*/

/*
 * read
 */
char *read_token_line(struct file_info *info, int column, char token)
{

  return NULL;
}

int next_buff_read_seek(struct file_info *info, int rb)
{
  int read_size = 0;
  if (!info)
    return EARG_NULL;

  if (strlen(info->buf) < info->buf_size)
    return -1;

  lseek(info->fd, -rb, SEEK_CUR);

  memset(info->buf, '\0', info->buf_size);
  read_size = read(info->fd, info->buf, info->buf_size);
  info->seek = 0;
  info->buf[info->buf_size] = EOF;

  if (read_size < 0)
    return read_size;

  /* Count */
  info->buf_read++;
  return 0;
}

int next_buff_read(struct file_info *info)
{

  if (!info)
    return EARG_NULL;

  if (read(info->fd, info->buf, info->buf_size) < 0)
    return -1;

  /* Count */
  info->buf_read++;
 
  return 0;

}


/*
 * read (next) string, split to '\n'
 * @info : file_info struct
 * return : result string
 */
char *read_next_line(struct file_info *info)
{/*{{{*/
  int read_size = 0;
  int i = 0;
  int start = -1;
  char tmp = '\0';

  // if (*info->buf == '\0' || 0 == info->buf_size) {
  if (*info->buf == '\0') {

read:
    /* read and save buf */
    memset(info->buf, '\0', info->buf_size);
    read_size = read(info->fd, info->buf, info->buf_size);
    info->seek = 0;
    i = 0;

    if (read_size <= 0)
      goto fail;

  } else {
    read_size = info->buf_size;
    i = info->seek;
  }
  start = i;

loop:
  /* lookup char */
  while (i < read_size) {

    /* pic char */
    tmp = *((info->buf) + i);

    /* new line count */
    if (tmp == '\n') {
      info->line++;
      goto ret;
    }

    if (tmp == '\0')
      goto fail;

    /* next */
    i++;
  }
  lseek(info->fd, start - i, SEEK_CUR);
  goto read;

fail:
  /* end */
  info->seek = -i;
  return NULL;

ret : 
  /* return string copy */
  strncpy(info->result, info->buf + start, i - start);
  info->result[i - start] = '\0';

  /* prevent re-find & Seek */
  info->seek = ++i;

  // printf("ret : %4d [%s] \n", i, info->result);
  return info->result;
}/*}}}*/

