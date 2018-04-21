/**

练习：Reader
实现一个 Reader 类型，它不断生成 ASCII 字符 'A' 的流。

*/

package main

import "golang.org/x/tour/reader"

type MyReader struct{}

// TODO: Add a Read([]byte) (int, error) method to MyReader.

func (r MyReader) Read(b []byte) (int, error){
    for i := 0; i < len(b); i++{
        b[i] = 'A'
    }
    return len(b), nil
}

func main(){
    reader.Validate(MyReader{})
}
