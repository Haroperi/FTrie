#include <stdio.h>
#include <iostream>
#include "ftrie.h"

using namespace std;

int main( int argc, char *argv[])
{
	CFtrie<wchar_t> *ftrie = new CFtrie<wchar_t>;
	vector<wchar_t> v, w, x;
	v.push_back(L'h');
	v.push_back(L'o');
	v.push_back(L'g');
	v.push_back(L'e');

	w.push_back(L'h');
        w.push_back(L'o');
        w.push_back(L'g');
        w.push_back(L'e');
        w.push_back(L'!');

	x.push_back(L'h');
        x.push_back(L'g');
        x.push_back(L'o');
        x.push_back(L'e');

	ftrie->insert(v);
	ftrie->insert(w);
	ftrie->show();


	if (ftrie->fuzzysearch(x, 2)) {
		puts("hoge");
	}
	else {
		puts("not found");
	}
}

