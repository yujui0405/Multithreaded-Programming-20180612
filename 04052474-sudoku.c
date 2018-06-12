#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 	11
#define PUZZLE_SIZE 	9
int subnum=2;

void *column_worker(void *param); 	/* 檢查一整行的執行緒 */
void *row_worker(void *param); 	/* 檢查一整列的執行緒 */
void *subfield_worker(void *param); 	/* 檢查一個3x3大小的執行緒 */

/* 以下矩陣內容助教檢查時會被換掉，請自行更改及測試 */
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,3,3,4,6,7,8,9,1,2},
			{-1,6,7,2,1,9,5,3,4,8},
			{-1,1,9,8,3,4,2,5,6,7},
			{-1,8,5,9,7,6,1,4,2,3},
			{-1,4,2,6,8,5,3,7,9,1},
			{-1,7,1,3,9,2,4,8,5,6},
			{-1,9,6,1,5,3,7,2,8,4},
			{-1,2,8,7,4,1,9,6,3,5},
			{-1,3,4,5,2,8,6,1,7,9}
		};
int status_map[NUMBER_OF_THREADS] = {0};   //全域整數陣列來記錄每個工作執行緒傳回的結果

/* data structure for passing data to threads */
typedef struct
{
	int thread_number;
	int x;
	int y;
} parameters;

int main(int argc, char *argv[])
{
	int i,j=0;
	int count = 0;
	int rv = 1;
	pthread_t workers[NUMBER_OF_THREADS];

	/* 建立一個執行緒負責檢查每一行是否包含1~9的數字 */
	parameters *data = (parameters *) malloc(sizeof(parameters));
	data->thread_number = count;
	data->x = 1;
	data->y = 1;

	pthread_create(&workers[data->thread_number], 0, row_worker, data);

	/* 建立一個執行緒負責檢查每一列是否包含1~9的數字 */
	count++;
	data = (parameters *) malloc(sizeof(parameters));	
	data->thread_number = count;
	data->x = 1;
	data->y = 1;
	pthread_create(&workers[data->thread_number], 0, column_worker, data);

	count++;
	/* 撰寫程式碼建立9個執行緒負責檢查每一個3x3的小方格是否包含1~9的數字 */
	for(i=1;i<=9;i++)
	{
	data = (parameters *) malloc(sizeof(parameters));	
	data->thread_number = count;
	data->x = 1;
	data->y = 1;
	pthread_create(&workers[data->thread_number], 0, subfield_worker, data);
	count++;
	}

	/* 撰寫程式碼等待所有工作執行緒結束 */
	for(i=0;i<11;i++)
	pthread_join(workers[i],NULL);
	/* 撰寫程式碼檢查status_map所放的值，確認該解答是否正確 */
	for(i=0;i<=10;i++)
	{
		if(status_map[i]==1)
		j++;
		else if(status_map[i]==0)
		{
			printf("Sudoku puzzle is invalid\n");
			break;
		}
	}
	if(j==11)
	printf("Sudoku puzzle is valid\n");
	return 0;
}
void *row_worker(void *params)
{
	int i,j, k, status;
	int map[PUZZLE_SIZE + 1] = {0};

	parameters* p = (parameters *)params;

	for (i = 1; i < PUZZLE_SIZE + 1; i++) {  //控制檢查第幾列
		for (j = 1; j < PUZZLE_SIZE + 1; j++) {
			map[puzzle[j][i]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

		if (status == 0)
			break;
		else {
			/* reinitialize the map */
			for (i = 0; i < PUZZLE_SIZE + 1; i++)
				map[i] = 0;//把所有重新歸為0 為下一列做準備
		}
	}
	if (status)
		status_map[p->thread_number] = 1;
	if(status_map[p->thread_number] == 1)
	printf("row_worker completion \n");

	pthread_exit(0);
}

void *column_worker(void *params)
{
	int i,j, k, status;
	int map[PUZZLE_SIZE + 1] = {0};

	parameters* p = (parameters *)params;

	for (i = 1; i < PUZZLE_SIZE + 1; i++) {  //控制檢查第幾列
		for (j = 1; j < PUZZLE_SIZE + 1; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

		if (status == 0)
			break;
		else {
			/* reinitialize the map */
			for (i = 0; i < PUZZLE_SIZE + 1; i++)
				map[i] = 0;//把所有重新歸為0 為下一列做準備
		}
	}
	if (status)
		status_map[p->thread_number] = 1;
	if(status_map[p->thread_number] == 1)
	printf("column_worker completion \n");

	pthread_exit(0);
}

void *subfield_worker(void *params)
{
	int i,j, k, status;
	int map[PUZZLE_SIZE + 1] = {0}; //map[10]

	parameters* p = (parameters *)params;
	if(subnum==2)/*設定9x9小方格的計算位置*/
	{
	for (i = 1; i < 4; i++) {  //控制檢查第幾列
		for (j = 1; j < 4; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==3)/*設定9x9小方格的計算位置*/
	{
	for (i = 1; i < 4; i++) {  //控制檢查第幾列
		for (j = 4; j < 7; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==4)/*設定9x9小方格的計算位置*/
	{
	for (i = 1; i < 4; i++) {  //控制檢查第幾列
		for (j = 7; j < 10; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==5)/*設定9x9小方格的計算位置*/
	{
	for (i = 4; i < 7; i++) {  //控制檢查第幾列
		for (j = 1; j < 4; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==6)/*設定9x9小方格的計算位置*/
	{
	for (i = 4; i < 7; i++) {  //控制檢查第幾列
		for (j = 4; j < 7; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==7)/*設定9x9小方格的計算位置*/
	{
	for (i = 4; i < 7; i++) {  //控制檢查第幾列
		for (j = 7; j < 10; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==8)/*設定9x9小方格的計算位置*/
	{
	for (i = 7; i < 10; i++) {  //控制檢查第幾列
		for (j = 1; j < 4; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==9)/*設定9x9小方格的計算位置*/
	{
	for (i = 7; i < 10; i++) {  //控制檢查第幾列
		for (j = 4; j < 7; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if(subnum==10)/*設定9x9小方格的計算位置*/
	{
	for (i = 7; i < 10; i++) {  //控制檢查第幾列
		for (j = 7; j < 10; j++) {
			map[puzzle[i][j]] = 1;	//ex.puzzle[1][1]=5  那在map[5]=1
		}
	}

		/* make sure the digits 1 .. 9 all appear */
		status = 1;
		for (k = 1; k < PUZZLE_SIZE + 1; k++) {
			if (map[k] == 0) {  //如果值為0  status會準備給0  代表1~9有缺
				status = 0;
				break;
			}
		}

	}
	if (status)
		status_map[p->thread_number] = 1;
	if(status_map[p->thread_number] == 1)
	printf("subfield_worker %d completion \n",subnum);
	subnum++;
	pthread_exit(0);
}
