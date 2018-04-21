/**

练习：图片
还记得之前编写的图片生成器吗？现在来另外编写一个，不过这次将会返回 image.Image 来代替 slice 的数据。

自定义的 Image 类型，要实现必要的方法，并且调用 pic.ShowImage。

Bounds 应当返回一个 image.Rectangle，例如 `image.Rect(0, 0, w, h)`。

ColorModel 应当返回 color.RGBAModel。

At 应当返回一个颜色；在这个例子里，在最后一个图片生成器的值 v 匹配 `color.RGBA{v, v, 255, 255}`。

*/

package main

import(
    "golang.org/x/tour/pic"
    "image"
    "image/color"
)

type Image struct{
    W, H int
}

func (img Image) Bounds() image.Rectangle{
    return image.Rect(0, 0, img.W, img.H)
}

func (img Image) ColorModel() color.Model{
    return color.RGBAModel
}

func (img Image) At(x, y int) color.Color{
    clr := (uint8)(x*y)
    return color.RGBA{clr, clr, clr, 255}
}

func main(){
    m := Image{30, 30}
    pic.ShowImage(m)
}
