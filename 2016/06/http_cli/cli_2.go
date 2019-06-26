package main

import (
        "fmt"
        "io"
        "io/ioutil"
        "net/http"
        "os"
        "time"
)

func closeResp(resp *http.Response) {
        _, err := io.Copy(ioutil.Discard, resp.Body)
        resp.Body.Close()
        if err != nil {
                fmt.Fprintf(os.Stderr, "fetch: reading: %v\n", err)
                os.Exit(1)
        }
}

func main() {
        for {
                resp1, err := http.Get("https://www.google.com/")
                if err != nil {
                        fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
                        os.Exit(1)
                }

                resp2, err := http.Get("https://www.facebook.com/")
                if err != nil {
                        fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
                        os.Exit(1)
                }

                time.Sleep(45 * time.Second)
                for _, v := range []*http.Response{resp1, resp2} {
                        closeResp(v)
                }
        }
}
