/**

练习：map
实现 WordCount。它应当返回一个含有 s 中每个 “词” 个数的 map。函数 wc.Test 针对这个函数执行一个测试用例，并输出成功还是失败。

你会发现 strings.Fields 很有帮助。

*/

package main

import(
    "golang.org/x/tour/wc"
    "strings"
)

func WordCount(s string) map[string]int{
    retMap := make(map[string]int)
    words := strings.Fields(s)
    for i := 0; i < len(words); i++{
        _, ok := retMap[words[i]]
        if ok {
            retMap[words[i]]++
        }else{
            retMap[words[i]] = 1
        }
    }

    return retMap
}

func main(){
    wc.Test(WordCount)
}
