/**



*/

package main

import "golang.org/x/tour/pic"

//import "/usr/local/go/misc/tour/src/golang.org/x/tour/pic/pic.go"

func Pic(dx, dy int)[][]uint8{
    ret := make([][]uint8, dy)
    for i := 0; i < dy;i++{
        ret [i] = make([]uint8, dx)
    }
    
    for x := 0; x < dx; x++{
        for y := 0; y < dy; y++{
            //ret[y][x] = uint8((x + y)/2)
            //ret[y][x] = uint8((x*y))
            ret[y][x] = uint8(x^y)
        }
    }
    
    return ret
}

func main(){
    pic.Show(Pic)
}
