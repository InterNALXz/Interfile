#include <stdio.h>
#include <magic.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>
#include <string.h>

int MagicDetectTest01(void)
{
    magic_t magic_ctx;
    char *result = NULL;
    char buffer[] = { 0x25, 'P', 'D', 'F', '-', '1', '.', '3', 0x0d, 0x0a};
    size_t buffer_len = sizeof(buffer);
    int retval = 0;

    magic_ctx = magic_open(0);	//这里可以选择其他模式，会出现不同展示效果
    if (magic_ctx == NULL) {
        printf("failure retrieving magic_ctx\n");
        return 0;
    }

    if (magic_load(magic_ctx, NULL) == -1) {
        printf("magic_load failure\n");
        goto end;
    }

    result = (char *)magic_buffer(magic_ctx, (void *)buffer, buffer_len);
    if (result == NULL || strncmp(result, "PDF document", 12) != 0) {
        printf("result %p:%s, not \"PDF document\": ", result,result?result:"(null)");
        goto end;
    }
    printf("result is %s\n", result);

    retval = 1;
end:
    magic_close(magic_ctx);
    return retval;
}

void main(int argc, char *argv[]){
    MagicDetectTest01();

    struct stat buf_s;

    stat(argv[1], &buf_s);    //获取文件状态

    size_t file_size = buf_s.st_size;

	char buf[file_size] ;		//未定义大小的buf不能直接初始化，需要下面手动初始化
    memset(&buf, 0, file_size);
    magic_t cookie = NULL;
    
    if (argc == 3 && *argv[2] == '0')
        cookie = magic_open(MAGIC_MIME);
    else
        cookie=magic_open(0);
    
	magic_load(cookie, NULL);
	const char *link = magic_file(cookie,argv[1]);
    printf("file info:%s\n", link);

    FILE *fp = fopen(argv[1], "r");
	FILE *fp1 = fopen("./test_file.r", "w");
	size_t i = 0;
	while(i < file_size){
		buf[i] = fgetc(fp);
		fputc(buf[i], fp1);
		i++;
	}
	printf("size is %ld\n", i);
	puts(magic_buffer(cookie, buf, file_size));
	magic_buffer(cookie, buf, 1);
 	puts(magic_file(cookie,"./test_file.r"));
    magic_close(cookie);
	fclose(fp1);
	fclose(fp);
}