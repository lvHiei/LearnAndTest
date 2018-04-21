/**

练习：Stringers
让 IPAddr 类型实现 fmt.Stringer 以便用点分格式输出地址。

例如，IPAddr{1, 2, 3, 4} 应当输出 "1.2.3.4"。

*/

package main

import "fmt"

type IPAddr [4]byte

// TODO: Add a "String() string" method to IPAddr.
func (ip IPAddr) String() string {
    return fmt.Sprintf("%v.%v.%v.%v", ip[0], ip[1], ip[2], ip[3])
}

func main(){
    addrs := map[string]IPAddr{
        "loopback":{127, 0, 0, 1},
        "googleDns":{8, 8, 8, 8},
    }

    for n, a := range addrs {
        fmt.Printf("%v : %v\n", n, a)
    }
}
