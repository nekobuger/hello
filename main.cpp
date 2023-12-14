#include<stdio.h>
#include<stdlib.h>
struct nodespace{
	int teskid;   // 任务号 
	int begin;    // 开始地址 
	int size;     // 大小 
	int status;   // 状态 0代表占用，1代表空闲 
	struct nodespace *next;  // 后指针 
};
 
void initNode(struct nodespace *p){
	if(p == NULL){	//如果为空则新创建一个 
		p = (struct nodespace*)malloc(sizeof(struct nodespace));
	}
	p->teskid = -1;
	p->begin = 0;
	p->size = 640;
	p->status = 1;
	p->next =NULL; 
}
 
/*
*  首次适应算法 
*/ 
void myMalloc1(int teskid,int size,struct nodespace *node){
	while(node != NULL){
		if(node->status == 1){  //空闲的空间 
			if(node->size > size){  //当需求小于剩余空间充足的情况 
				//分配后剩余的空间 
				struct nodespace *p = (struct nodespace*)malloc(sizeof(struct nodespace));
				p->begin = node->begin + size;
				p->size = node->size - size;
				p->status = 1;
				p->teskid = -1;
				//分配的空间 
				node->teskid = teskid; 
				node->size = size;
				node->status = 0;
				//改变节点的连接 
				p->next = node->next; 
				node->next = p;
				break; 
			}else if(node->size == size){ //需求空间和空闲空间大小相等时 
				node->teskid = teskid; 
				node->size = size;
				node->status = 0;
				break;
			}	
		}
		if(node->next == NULL){
			printf("分配失败，没有足够的空间！\n");
			break;
		}
		node = node->next;
	}
} 
 
/*
* 最佳适应算法 
*/
void myMalloc2(int teskid,int size,struct nodespace *node){
	//最佳块指针 
	struct nodespace *q = NULL;
	//首先找到第一个满足条件的空闲块 
	while(node != NULL){
		if(node->status == 1 && node->size >= size){
			q = node;
			break;
		}
		//如果下一个为空则说明没有空闲区可以分配 
		if(node->next == NULL){
			printf("分配失败，没有足够的空间！\n");
			break;
		} else{
			node = node->next;	
		}
		
	} 
	//遍历寻找最佳的空闲块 
	while(node != NULL){
		if(node->status == 1 && node->size >= size && node->size < q->size){  //空闲的空间 
			q = node;
		}
		node = node->next;
	}
	if(q->size > size){  			//最佳空闲块的大小大于需求大小 
		//分配后剩余的空间 
		struct nodespace *p = (struct nodespace*)malloc(sizeof(struct nodespace));
		p->begin = q->begin + size;
		p->size = q->size - size;
		p->status = 1;
		p->teskid = -1;
		//分配的空间 
		q->teskid = teskid; 
		q->size = size;
		q->status = 0;
		//改变节点的连接 
		p->next = q->next; 
		q->next = p;
	}else if(q->size == size){  	//最佳空闲块空间大小和需求相等
		q->teskid = teskid; 
		q->size = size;
		q->status = 0;
	}		
}
 
void myFree(int teskid,struct nodespace *node){
	if(node->next == NULL && node->teskid == -1){
		printf("还没有分配任何任务！\n");
	}
	
	while(node != NULL){
		if(node->status == 1 && node->next->status ==0 && node->next->teskid == teskid){  //释放空间的上一块空间空闲时 
			node->size = node->size + node->next->size;
			struct nodespace *q = node->next;
			node->next = node->next->next;
			free(q);
			if(node->next->status == 1){ //下一个空间是空闲空间时 
				node->size = node->size + node->next->size;
				struct nodespace *q = node->next;
				node->next = node->next->next;
				free(q);
			}
			break;
		}else if(node->status == 0 && node->teskid == teskid){  //释放空间和空闲空间不连续时 
			node->status = 1;
			node->teskid = -1;
			if(node->next != NULL && node->next->status == 1){ //下一个空间是空闲空间时 
				node->size = node->size + node->next->size;
				struct nodespace *q = node->next;
				node->next = node->next->next;
				free(q);
			}
			break;
		}else if(node->next == NULL){  //任务id不匹配时 
			printf("没有此任务！\n");
			break;
		}
		node = node->next;
	}
	
	 
}
 
void printNode(struct nodespace *node){
	printf("                        内存情况                        \n"); 
	printf(" -------------------------------------------------------\n");
	printf("| 起始地址\t结束地址\t大小\t状态\t任务id\t|\n");
	while(node != NULL){
		if(node->status==1){
			printf("| %d\t\t%d\t\t%dKB\tfree\t 无\t|\n", node->begin + 1, node->begin+node->size, node->size);
		}else{
			printf("| %d\t\t%d\t\t%dKB\tbusy\t %d\t|\n", node->begin + 1, node->begin+node->size, node->size, node->teskid);
		}
		node = node->next;
	}
	printf(" -------------------------------------------------------\n");
}
 
void destory(struct nodespace *node){
	struct nodespace *q = node;
	while(node != NULL){
		node = node->next;
		free(q);
		q = node;
	}
}
 
void menu(){
	printf("1.分配内存\n");
	printf("2.回收内存\n");
	printf("3.查看内存情况\n");
	printf("4.退出\n");
	printf("请输入选项:");
}
 
int main(){
	// node为整个空间 
	struct nodespace *init = (struct nodespace*)malloc(sizeof(struct nodespace));
	struct nodespace *node = NULL;
	initNode(init);			//初始化主链 
	node = init; 			//指向链表头 
	int option; 
	int teskid;
	int size;
	while(1){
		printf("请选择模式：\n 1.演示模式\n 2.自由模式\n 3.退出\n");
		scanf("%d",&option);
		if(option == 1){	//演示模式 
			while(1){		//循环选择实现的算法 
				printf("请选择算法：\n 1.首次适应算法\n 2.最佳适应算法\n 3.退出\n");
				scanf("%d",&option);
				if(option == 1){			//首次适应算法 
					printf("作业1 申请130 KB\n");
					myMalloc1(1,130,node);		//作业1 申请130 KB
					printNode(node);
					printf("作业2 申请60 KB\n");
					myMalloc1(2,60,node);		//作业2 申请60 KB
					printNode(node);
					printf("作业3 申请100 KB\n");
					myMalloc1(3,100,node);		//作业3 申请100 KB
					printNode(node);
					printf("作业2 释放60 KB\n");
					myFree(2,node);			//作业2 释放60 KB
					printNode(node);
					printf("作业4 申请200 KB\n");
					myMalloc1(4,200,node);		//作业4 申请200 KB
					printNode(node);
					printf("作业3 释放100 KB\n");
					myFree(3,node);			//作业3 释放100 KB
					printNode(node);
					printf("作业1 释放130 KB\n");
					myFree(1,node);			//作业1 释放130 KB
					printNode(node);
					printf("作业5 申请140 KB\n");
					myMalloc1(5,140,node);		//作业5 申请140 KB
					printNode(node);
					printf("作业6 申请60 KB\n");
					myMalloc1(6,60,node);		//作业6 申请60 KB
					printNode(node);
					printf("作业7 申请50 KB\n");
					myMalloc1(7,50,node);		//作业7 申请50 KB
					printNode(node);
					printf("作业6 释放60 KB\n");
					myFree(6,node);			//作业6 释放60 KB
					printNode(node);
					destory(node);	//销毁链表
					initNode(init);	//重新初始化 
					node = init;	//重新指向开头 
				}else if(option == 2){		//最佳适应算法 
					printf("作业1 申请130 KB\n");
					myMalloc2(1,130,node);		//作业1 申请130 KB
					printNode(node);
					printf("作业2 申请60 KB\n");
					myMalloc2(2,60,node);		//作业2 申请60 KB
					printNode(node);
					printf("作业3 申请100 KB\n");
					myMalloc2(3,100,node);		//作业3 申请100 KB
					printNode(node);
					printf("作业2 释放60 KB\n");
					myFree(2,node);			//作业2 释放60 KB
					printNode(node);
					printf("作业4 申请200 KB\n");
					myMalloc2(4,200,node);		//作业4 申请200 KB
					printNode(node);
					printf("作业3 释放100 KB\n");
					myFree(3,node);			//作业3 释放100 KB
					printNode(node);
					printf("作业1 释放130 KB\n");
					myFree(1,node);			//作业1 释放130 KB
					printNode(node);
					printf("作业5 申请140 KB\n");
					myMalloc2(5,140,node);		//作业5 申请140 KB
					printNode(node);
					printf("作业6 申请60 KB\n");
					myMalloc2(6,60,node);		//作业6 申请60 KB
					printNode(node);
					printf("作业7 申请50 KB\n");
					myMalloc2(7,50,node);		//作业7 申请50 KB
					printNode(node);
					printf("作业6 释放60 KB\n");
					myFree(6,node);			//作业6 释放60 KB
					printNode(node);
					destory(node);	//销毁链表
					initNode(init);	//重新初始化 
					node = init;	//重新指向开头 
				}else if(option == 3){		//退出
					break;
				}else{
					printf("您的输入有误，请重新输入！\n"); 
				}
			} 	
		}else if(option == 2){	//自由模式 
			while(1){		//循环选择使用的算法 
				printf("请选择算法：\n 1.首次适应算法\n 2.最佳适应算法\n 3.退出\n");
				scanf("%d",&option);
				int n = option;		//标记选择的算法，n == 1 表示首次适应算法, n == 2表示最佳适应算法 
				if(option != 3){
					while(1){
						menu();		//打印想要进行的操作 
						scanf("%d",&option);
						if(option == 1 && n == 1){			//首次适应 
							printf("请输入任务id以及申请的空间大小:\n");
							scanf("%d%d",&teskid,&size);
							myMalloc1(teskid,size,node);
							printNode(node);
						}else if(option == 1 && n == 2){	//最佳适应 
							printf("请输入任务id以及申请的空间大小:\n");
							scanf("%d%d",&teskid,&size);
							myMalloc2(teskid,size,node);
							printNode(node);
						}else if(option == 2){
							printf("请输入任务id:\n");
							scanf("%d",&teskid);
							myFree(teskid,node);
							printNode(node);
						}else if(option == 3){
							printNode(node);
						}else if(option == 4){
							destory(node);	//销毁链表
							initNode(init);	//重新初始化 
							node = init;	//重新指向开头 
							break;
						}else{
							printf("您的输入有误，请重新输入！\n");
							continue;
						}
					}
				}else if(option == 3){
					destory(node);	//销毁链表
					initNode(init);	//重新初始化 
					node = init;	//重新指向开头 
					break;
				}
				else{
					printf("您的输入有误，请重新输入！\n");
				}
			} 
				
		}else if(option == 3){	//退出  
			destory(node);
			return 0;
		}else {
			printf("您的输入有误，请重新输入！\n");
			continue;
		}
	}
	return 0;
}
