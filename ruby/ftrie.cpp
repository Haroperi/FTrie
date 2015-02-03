extern "C" {
#include "ruby.h"
#include "ruby/ruby.h"
#include <stdlib.h>
#include <stdio.h>
}


#include "../ftrie.h"

struct ftriedata
{
        CFtrie<wchar_t> ftrie;
};

template<typename data_t>
void rarray2vector(const VALUE &arr, vector<data_t> &v)
{
        long len = RARRAY_LEN(arr);
        v.clear();
        for (int i = 0; i < len; i++) {
                v.push_back(rb_ary_entry(arr, i));
        }
}


extern "C" {

VALUE ftrie_m(VALUE self, VALUE str);
VALUE search_m(VALUE self, VALUE arr);
VALUE fuzzysearch_m(VALUE self, VALUE arr, VALUE cost);
VALUE insert_m(VALUE self, VALUE arr);
VALUE remove_m(VALUE self, VALUE arr);

static void
free_ftrie(ftriedata *ptr)
{
        delete ptr;
}

static VALUE
ftrie_alloc(VALUE klass)
{
        return Data_Wrap_Struct(klass, 0, free_ftrie, new ftriedata);
}

void Init_ftrie(void)
{
        VALUE cftrie = rb_define_class("Ftrie", rb_cObject);
        rb_define_alloc_func(cftrie, ftrie_alloc);

        rb_define_method(cftrie, "ftrie",
        		reinterpret_cast<VALUE (*)(...)>(ftrie_m), 1);
        rb_define_method(cftrie, "insert",
        		reinterpret_cast<VALUE (*)(...)>(insert_m), 1);
        rb_define_method(cftrie, "search",
        		reinterpret_cast<VALUE (*)(...)>(search_m), 1);
        rb_define_method(cftrie, "fuzzysearch",
        		reinterpret_cast<VALUE (*)(...)>(fuzzysearch_m), 2);
        rb_define_method(cftrie, "remove",
        		reinterpret_cast<VALUE (*)(...)>(remove_m), 1);
}

VALUE ftrie_m(VALUE self, VALUE str)
{
        Check_Type(str, T_STRING);

        printf("%s\n", StringValuePtr(str));
        return Qnil;
}

VALUE search_m(VALUE self, VALUE arr)
{
        Check_Type(arr, T_ARRAY);
        vector<wchar_t> v;
        rarray2vector(arr, v);

        ftriedata *pdata = NULL;
        Data_Get_Struct(self, ftriedata, pdata);
        if (pdata->ftrie.search(v))
                return Qtrue;
        return Qfalse;
}

VALUE insert_m(VALUE self, VALUE arr)
{
        Check_Type(arr, T_ARRAY);
        vector<wchar_t> v;
        rarray2vector(arr, v);

        ftriedata *pdata = NULL;
        Data_Get_Struct(self, ftriedata, pdata);
        pdata->ftrie.insert(v);
        // TODO: もし失敗したらQnilを返す
        return Qtrue;
}

VALUE remove_m(VALUE self, VALUE arr)
{
        Check_Type(arr, T_ARRAY);
        vector<wchar_t> v;
        rarray2vector(arr, v);

        ftriedata *pdata = NULL;
        Data_Get_Struct(self, ftriedata, pdata);
        pdata->ftrie.remove(v);
        return Qnil;
}

VALUE fuzzysearch_m(VALUE self, VALUE arr, VALUE cost)
{
        Check_Type(arr, T_ARRAY);
        Check_Type(cost, T_FIXNUM);
        vector<wchar_t> v;
        rarray2vector(arr, v);

        ftriedata *pdata = NULL;
        Data_Get_Struct(self, ftriedata, pdata);
        if (pdata->ftrie.fuzzysearch(v, FIX2INT(cost)))
                return Qtrue;
        return Qfalse;

}

} // extern "C"
