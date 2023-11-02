class Toc {
    static kTitle = "Table of Contents"

    static getToc() {
        return document.querySelector(".contents > .toc")
    }

    static createEmptyToc() {
        let div = document.createElement("div")
        div.classList.add("toc")
        div.setAttribute("style", "position:absolute;right:1vw;top:10vh")

        let h3 = div.appendChild(document.createElement("h3"))
        h3.textContent = this.kTitle

        let ul = div.appendChild(document.createElement("ul"))

        return div
    }

    static createTocItem(level, name, id) {
        let li = document.createElement("li")
        li.classList.add("level" + level)
        let a = li.appendChild(document.createElement("a"))
        a.setAttribute("href", `#${id}`)
        a.textContent = name
        return li
    }
}
