/**

练习：斐波纳契闭包
现在来通过函数做些有趣的事情。

实现一个 fibonacci 函数，返回一个函数（一个闭包）可以返回连续的斐波纳契数。

*/

package main

import "fmt"

// fibonacci 函数会返回一个返回 int 的函数。
func fibonacci() func()int{
    f0 := 0
    f1 := 1
    return func()int{
        f0, f1 = f1, f0 + f1
        return f1
    }
}

func main(){
    f := fibonacci()
    for i := 0; i < 10; i++{
        fmt.Println(f())
    }
}
