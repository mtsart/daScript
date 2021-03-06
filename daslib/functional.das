options indenting = 4

module functional

[private]
def filter_any(var src:iterator<auto(TT)>; blk )
    static_if !typeinfo(is_ref_type type<TT>)
        return <- generator<TT -&> () <| $ ()
            for w in src
                if invoke(blk,w)
                    yield w
            return false
    else
        return <- generator<TT&> () <| $ ()
            for w in src
                if invoke(blk,w)
                    yield w
            return false

def filter(var src:iterator<auto(TT)>; blk:lambda<(what:TT -&):bool> )
    return <- filter_any(src,blk)

// filter with function
def filter(var src:iterator<auto(TT)>; blk:function<(what:TT -&):bool> )
    return <- filter_any(src,blk)

[private]
def map_any(var src:iterator<auto(TT)>; blk)
    static_if typeinfo(can_copy type<QQ>)
        return <- generator<QQ -&> () <| $ ()
            for w in src
                yield invoke(blk,w)
            return false
    static_elif typeinfo(can_move type<QQ>)
        return <- generator<QQ -&> () <| $ ()
            for w in src
                yield <- invoke(blk,w)
            return false
    else
        concept_assert(false,"can't copy or move return type")
        var dummy : iterator<QQ>
        return <- dummy


// map with lambda
def map(var src:iterator<auto(TT)>; blk:lambda<(what:TT -&):auto(QQ)>)
    return <- map_any(src,blk)

// map with function
def map(var src:iterator<auto(TT)>; blk:function<(what:TT -&):auto(QQ)>)
    return <- map_any(src,blk)

// reduce value, any invokable
[private]
def reduce_any(it:iterator<auto(TT)>;functor)
    var left : TT -&
    var first = true
    for right in it
        if first
            first = false
            left = right
        else
            left = invoke(functor,left,right)
    if first
        panic("can't reduce empty sequence")
    return left

// reduce with lambda
def reduce(it:iterator<auto(TT)>;blk:lambda<(left,right:TT -&):TT const -&>)
    return reduce_any(it,blk)

// reduce with function
def reduce(it:iterator<auto(TT)>;blk:function<(left,right:TT -&):TT const -&>)
    return reduce_any(it,blk)

// reduce with block
def reduce(it:iterator<auto(TT)>;blk:block<(left,right:TT -&):TT const -&>)
    return reduce_any(it,blk)

// summ of all elements
//  same as reduce(it, @(a,b) => a + b)
def sum(it:iterator<auto(TT)>)
    var temp : TT -&
    for x in it
        temp += x
    return temp

// any
def any ( it )
    static_if !typeinfo(is_iterable it)
        concept_assert(false,"can't ANY non-iterable type")
        return false
    else
        for x in it
            if x
                return true
        return false

// all
def all ( it )
    static_if !typeinfo(is_iterable it)
        concept_assert(false,"can't ALL non-iterable type")
        return false
    else
        for x in it
            if !x
                return false
        return true

def cycle(var src:iterator<auto(TT)>)
    static_if !typeinfo(is_ref_type type<TT>)
        return <- generator<TT -&> () <| $ ()
            var arr : array<TT -&>
            for x in src
                yield x
                push(arr,x)
            while true
                for x in arr
                    yield x
            return false
        finally
            __builtin_array_clear_lock(arr)
    else
        return <- generator<TT &> () <| $ ()
            var arr : array<TT -&>
            for x in src
                yield x
                push_clone(arr,x)
            while true
                for x in arr
                    yield x
            return false
        finally
            __builtin_array_clear_lock(arr)

def islice( var src : iterator<auto(TT)>; start,stop : int )
    static_if !typeinfo(is_ref_type type<TT>)
        return <- generator<TT -&> () <| $ ()
            for x,i in src,range(0,stop)
                if i>=start
                    yield x
            return false
    else
        return <- generator<TT &> () <| $ ()
            for x,i in src,range(0,stop)
                if i>=start
                    yield x
            return false

// [[ start; start+step; start+2*step ]]
[generic]
def count(var start,step:int) : iterator<int>
    return <- each() <| @(var result:int&) : bool
        result = start
        start += step
        return true

// [[ value; value; value; .... count times ]]
def repeat(value : auto(TT); var count : int = 0)
    return <- generator<TT> () <| $ ()
        if count == 0
            while true
                yield value
        for x in range(0,count)
            yield value
        return false

def is_equal(a,b)
    return a == b

def is_not_equal(a,b)
    return a != b

def not(x)
    return !x

def echo(var x;extra : string = "\n")
    print("{x}{extra}")
    return <- x
