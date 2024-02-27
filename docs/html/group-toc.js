class GroupToc {
    static init() {
        $(function() {
            $(document).ready(function() {
                GroupToc.createToC()
            })
        })
    }

    static isGroupPage() {
        let file = window.location.pathname.split("/").pop()
        return file.substring(0, 7) === "group__";

    }

    static createToC() {
        if (!this.isGroupPage()) {
            return
        }

        if (Toc.getToc()) {
            return
        }


        let toc = Toc.createEmptyToc()
        let ul = toc.querySelector("ul")

        document.querySelectorAll("h2.groupheader .anchor").forEach((node, node_index) => {
            let item = Toc.createTocItem(1, node.nextSibling.textContent, node.getAttribute("id"))
            ul.appendChild(item)

            let parent = node.closest("tbody")
            if (parent) {
                parent.querySelectorAll("h3.groupheader .anchor").forEach((level2) => {
                    let item = Toc.createTocItem(2, level2.nextSibling.textContent, level2.id)
                    ul.appendChild(item)
                })
            }
        })

        let contents = document.querySelector(".contents")
        contents.insertBefore(toc, contents.firstChild)
    }
}
