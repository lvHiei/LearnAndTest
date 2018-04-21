/**

练习：等价二叉树
（本习题较长，将在 下一页 上继续。）

可以用多种不同的二叉树的叶子节点存储相同的数列值。例如，这里有两个二叉树保存了序列 1，1，2，3，5，8，13。


用于检查两个二叉树是否存储了相同的序列的函数在多数语言中都是相当复杂的。这里将使用 Go 的并发和 channel 来编写一个简单的解法。

这个例子使用了 tree 包，定义了类型：

type Tree struct {
    Left  *Tree
    Value int
    Right *Tree
}

练习：等价二叉树
1. 实现 Walk 函数。

2. 测试 Walk 函数。

函数 tree.New(k) 构造了一个随机结构的二叉树，保存了值 k，2k，3k，...，10k。 创建一个新的 channel ch 并且对其进行步进：

go Walk(tree.New(1), ch)
然后从 channel 中读取并且打印 10 个值。应当是值 1，2，3，...，10。

3. 用 Walk 实现 Same 函数来检测是否 t1 和 t2 存储了相同的值。

4. 测试 Same 函数。

Same(tree.New(1), tree.New(1)) 应当返回 true，而 Same(tree.New(1), tree.New(2)) 应当返回 false。


*/

package main

import(
    "fmt"
    "golang.org/x/tour/tree"
)

func StrideStep(t *tree.Tree, ch chan int){
    if t != nil{
        StrideStep(t.Left, ch)
        ch <- t.Value
        StrideStep(t.Right, ch)
    }
}

func Walk(t *tree.Tree, ch chan int){
    StrideStep(t, ch)
    close(ch)
}

func Same(t1, t2 *tree.Tree) bool{
    ch1 := make(chan int, 10)    
    ch2 := make(chan int, 10)

    go Walk(t1, ch1)
    go Walk(t2, ch2)

    for i := range ch1{
        if i != <-ch2{
            return false
        }
    }

    return true
}

func main(){
    ch := make(chan int, 10)
    go Walk(tree.New(1), ch)

    for i := range ch{
        fmt.Println(i)
    }

    fmt.Println(Same(tree.New(1), tree.New(1)))
    fmt.Println(Same(tree.New(1), tree.New(22)))

}
