class AddPermalink {

    static addAnchorTo(node, node_id) {
        let anchor = node.querySelector(":scope > a[id]")
        if (!anchor) {
            anchor = document.createElement("a")
            node.insertBefore(anchor, node.firstChild)
        }

        let id = anchor.getAttribute(anchor, "id")
        if (!id) {
            id = node_id;
            anchor.setAttribute("id", id)
        }

        if (!anchor.getAttribute("name")) {
            anchor.setAttribute("name", id)
        }

        if (!anchor.classList.contains("anchor")) {
            anchor.classList.add("anchor")
        }

        return id
    }

    static addPermalinkTo(node, default_id) {
        let id = this.addAnchorTo(node, default_id)

        let anchorlink = node.querySelector(`:scope > a[href]`)
        if (!anchorlink) {
            anchorlink = document.createElement("a")
            node.appendChild(anchorlink)
        }

        anchorlink.setAttribute("href", `#${id}`)
        anchorlink.setAttribute("title", window.DoxygenAwesomeParagraphLink.title)
        anchorlink.classList.add("anchorlink")
        anchorlink.innerHTML = window.DoxygenAwesomeParagraphLink.icon
    }

    static getValidID(text) {
        return text.replace(/\W/g,'_');
    }
}

window.AddPermalink = AddPermalink
