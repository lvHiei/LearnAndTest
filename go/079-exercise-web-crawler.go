/**

练习：Web 爬虫
在这个练习中，将会使用 Go 的并发特性来并行执行 web 爬虫。

修改 Crawl 函数来并行的抓取 URLs，并且保证不重复。

提示：你可以用一个 map 来缓存已经获取的 URL，但是需要注意 map 本身并不是并发安全的！

*/


package main

import(
    "fmt"
    "sync"
    "time"
)

var (
    gotedUrls    map[string]int = make(map[string]int)
    mutex        sync.Mutex
)

type Fetcher interface{
    // Fetch 返回 URL 的 body 内容，并且将在这个页面上找到的 URL 放到一个 slice 中。
    Fetch(url string)(body string, urls []string, err error)
}

// Crawl 使用 fetcher 从某个 URL 开始递归的爬取页面，直到达到最大深度。
func Crawl(url string, depth int, fetcher Fetcher){
    if(depth <= 0){
        return
    }

    body, urls, err := fetcher.Fetch(url)

    if err != nil{
        fmt.Println(err)
        return
    }

    mutex.Lock()
    if gotedUrls[url] == 0{
        gotedUrls[url]++
        fmt.Printf("found: %s %q\n", url, body)
        depth--
        for _,u := range urls{
            go Crawl(u, depth, fetcher)
        }
    }

    mutex.Unlock()
    return
}

func main(){
    Crawl("http://golang.org/", 4, fetcher)

    time.Sleep(time.Second)

    for k := range gotedUrls{
        fmt.Println("*****", k)
    }
}

// fakeFetcher 是返回若干结果的 Fetcher。
type fakeFetcher map[string]*fakeResult

type fakeResult struct{
    body string
    urls []string
}

func (f fakeFetcher) Fetch(url string)(string, []string, error){
    if res, ok := f[url]; ok{
        return res.body, res.urls, nil
    }

    return "", nil, fmt.Errorf("not found: %s", url)
}

// fetcher 是填充后的 fakeFetcher。
var fetcher = fakeFetcher{
    "http://golang.org/": &fakeResult{
        "The Go Programming Language",
        []string{
            "http://golang.org/pkg/",
            "http://golang.org/cmd/",
        },
    },
    "http://golang.org/pkg/": &fakeResult{
        "Packages",
        []string{
            "http://golang.org/",
            "http://golang.org/cmd/",
            "http://golang.org/pkg/fmt/",
            "http://golang.org/pkg/os/",
        },
    },
    "http://golang.org/pkg/fmt/": &fakeResult{
        "Package fmt",
        []string{
            "http://golang.org/",
		    "http://golang.org/pkg/",
	    },
    },
    "http://golang.org/pkg/os/": &fakeResult{
        "Package os",
        []string{
            "http://golang.org/",
            "http://golang.org/pkg/",
         },
	},
}
