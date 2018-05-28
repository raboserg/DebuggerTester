#pragma once
#include "stdafx.h"

struct List {
	typedef struct node {
		DWORD value;
		struct node *next;
	} Node;
	Node *head = NULL;
	void Add(DWORD data);
	void ClearList();
	void PrintList();
};
