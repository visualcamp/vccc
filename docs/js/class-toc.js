class ClassToc {
    static init() {
        $(function() {
            $(document).ready(function() {
                ClassToc.createToC()
            })
        })
    }


    static createToC() {
        let toc = document.querySelector(".contents > .toc")
        if (toc)
            return

        let div = document.createElement("div")
        div.classList.add("toc")
        div.setAttribute("style", "position:absolute;right:1vw;top:10vh")

        let h3 = div.appendChild(document.createElement("h3"))
        h3.textContent = "Table of Contents"

        let ul = div.appendChild(document.createElement("ul"))

        document.querySelectorAll("h2.groupheader .anchor").forEach((level1, node_index) => {
            let id = level1.getAttribute("id")

            let li = ul.appendChild(document.createElement("li"))
            li.classList.add("level1")
            let a = li.appendChild(document.createElement("a"))
            a.setAttribute("href", `#${id}`)
            a.textContent = level1.nextSibling.textContent

            let parent = level1.closest("tbody")
            if (parent) {
                parent.querySelectorAll("h3.groupheader .anchor").forEach((level2) => {
                    let id = level2.getAttribute("id")

                    let li = ul.appendChild(document.createElement("li"))
                    li.classList.add("level2")
                    let a = li.appendChild(document.createElement("a"))
                    a.setAttribute("href", `#${id}`)
                    a.textContent = level2.nextSibling.textContent
                })
            }
        })

        let contents = document.querySelector(".contents")
        contents.insertBefore(div, contents.firstChild)
        // contents.parentNode.appendChild(div)
    }
}
