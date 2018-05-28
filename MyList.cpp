//
#include "stdafx.h"
#include "MyList.h"

void List::Add(DWORD data) {
	Node *link = (Node*) malloc(sizeof(Node));
	link->value = data;
	link->next = head;
	head = link;
}

void List::ClearList() {
	Node *prev = NULL;
	while (head->next) {
		prev = head;
		head = head->next;
		free(prev);
	}
	free(head);
}

void List::PrintList() {
	Node *ptr = head;
	printf("\n[head] =>");
	while (ptr) {
		printf(" Ox%08x =>", ptr->value);
		ptr = ptr->next;
	}
	printf(" [null]\n");
}