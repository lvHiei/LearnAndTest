/**

结构体指针
结构体字段可以通过结构体指针来访问。

通过指针间接的访问是透明的。

*/

package main

import "fmt"

type Vertex struct{
    X int 
    Y int
    Z int 
}

func main(){
    v := Vertex{1, 2, 3}
    p := &v
    v.X = 1e9
    p.Y = 44
    (*p).Z = 33
    fmt.Println(v)
}
