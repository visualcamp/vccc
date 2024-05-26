class ClassToc {
    static init() {
        $(function() {
            $(document).ready(function() {
                ClassToc.createToC()
            })
        })
    }

    static isClassPage() {
        let file = window.location.pathname.split("/").pop()
        if (file.substring(0, 5) === "class" || file.substring(0, 6) === "struct") {
            return true
        }
        return false
    }

    static createToC() {
        if (!this.isClassPage()) {
            return
        }

        if (Toc.getToc()) {
            return
        }

        let toc = Toc.createEmptyToc()
        let ul = toc.querySelector("ul")

        document.querySelectorAll("h2.groupheader .anchor").forEach((level1, node_index) => {
            let item = Toc.createTocItem(1, level1.nextSibling.textContent, level1.getAttribute("id"))
            ul.appendChild(item)

            let parent = level1.closest("tbody")
            if (parent) {
                parent.querySelectorAll("h3.groupheader .anchor").forEach((level2) => {
                    let item = Toc.createTocItem(2, level2.nextSibling.textContent, level2.getAttribute("id"))
                    ul.appendChild(item)
                })
            }
        })

        let contents = document.querySelector(".contents")
        contents.insertBefore(toc, contents.firstChild)
    }
}
