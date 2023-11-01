/**

 Doxygen Awesome
 https://github.com/jothepro/doxygen-awesome-css

 MIT License

 Copyright (c) 2022 - 2023 jothepro

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 */

var kDoxygenAwesomePermanentLinkIcon = `<svg xmlns="http://www.w3.org/2000/svg" height="20px" viewBox="0 0 24 24" width="20px"><path d="M0 0h24v24H0V0z" fill="none"/><path d="M17 7h-4v2h4c1.65 0 3 1.35 3 3s-1.35 3-3 3h-4v2h4c2.76 0 5-2.24 5-5s-2.24-5-5-5zm-6 8H7c-1.65 0-3-1.35-3-3s1.35-3 3-3h4V7H7c-2.76 0-5 2.24-5 5s2.24 5 5 5h4v-2zm-3-4h8v2H8z"/></svg>`
var kDoxygenAwesomePermanentLinkTitle = "Permanent Link"

class DoxygenAwesomePermanentLink {
    // Icon from https://fonts.google.com/icons
    // Licensed under the Apache 2.0 license:
    // https://www.apache.org/licenses/LICENSE-2.0.html
    static icon = `<svg xmlns="http://www.w3.org/2000/svg" height="20px" viewBox="0 0 24 24" width="20px"><path d="M0 0h24v24H0V0z" fill="none"/><path d="M17 7h-4v2h4c1.65 0 3 1.35 3 3s-1.35 3-3 3h-4v2h4c2.76 0 5-2.24 5-5s-2.24-5-5-5zm-6 8H7c-1.65 0-3-1.35-3-3s1.35-3 3-3h4V7H7c-2.76 0-5 2.24-5 5s2.24 5 5 5h4v-2zm-3-4h8v2H8z"/></svg>`
    static title = "Permanent Link"
    static init() {
        $(function() {
            $(document).ready(function() {
                DoxygenAwesomePermanentLink.updateParagraph();
                DoxygenAwesomePermanentLink.updateMemberDecls();
                DoxygenAwesomePermanentLink.MoveGroups();
            })
        })
    }

    static updateParagraph() {
        document.querySelectorAll(".contents a.anchor[id], .contents .groupheader > a[id]").forEach((node) => {
            let anchorlink = document.createElement("a")
            anchorlink.setAttribute("href", `#${node.getAttribute("id")}`)
            anchorlink.setAttribute("title", DoxygenAwesomePermanentLink.title)
            anchorlink.classList.add("anchorlink")
            node.classList.add("anchor")
            anchorlink.innerHTML = DoxygenAwesomePermanentLink.icon
            node.parentElement.appendChild(anchorlink)
        })
    }

    static addAnchorTo(node, node_index) {
        let anchor = node.querySelector(":scope > a[id]")
        if (!anchor) {
            anchor = document.createElement("a")
            node.insertBefore(anchor, node.firstChild)
        }

        let id = anchor.getAttribute(anchor, "id")
        if (!id) {
            id = DoxygenAwesomePermanentLink.getValidID(node.textContent)
            if (id.length === 0)
                id = "_" + node_index
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

    static addPermalinkTo(node, node_index) {
        let id = this.addAnchorTo(node, node_index)

        let anchorlink = node.querySelector(`:scope > a[href]`)
        if (!anchorlink) {
            anchorlink = document.createElement("a")
            node.appendChild(anchorlink)
        }

        anchorlink.setAttribute("href", `#${id}`)
        anchorlink.setAttribute("title", DoxygenAwesomePermanentLink.title)
        anchorlink.classList.add("anchorlink")
        anchorlink.innerHTML = DoxygenAwesomePermanentLink.icon
    }

    static updateMemberDecls() {
        document.querySelectorAll(".memberdecls .groupheader").forEach((node, node_index) => {
            let has_child = node.querySelector(".anchor") != null;
            if (!has_child) {
                this.addPermalinkTo(node, node_index)
            }
        })
    }

    static MoveGroups() {
        document.querySelectorAll(`.memberdecls [id="groups"]`).forEach((node) => {
            node.closest(`tbody`).querySelectorAll(`tr[class*="memitem"]`).forEach((node) => {
                let category = this.GetGroupCategory(node.getAttribute("class"));
                let category_id;

                switch (category) {
                    case "func":
                        category_id = "func-members"
                        break

                    case "class":
                        category_id = "nested-classes"
                        break

                    default:
                        category_id = category
                        break
                }

                let memdecl = this.getMemDecl(category_id)
                if (!memdecl)
                    memdecl = DoxygenAwesomePermanentLink.addMemDecl(category_id);

                let next = node.nextSibling ? node.nextSibling.nextSibling : node.nextSibling
                memdecl.appendChild(node)

                while (next) {
                    if (next.classList.contains("memitem"))
                        break
                    memdecl.appendChild(next)
                    next = next.nextSibling ? next.nextSibling.nextSibling : next.nextSibling
                }
            })

            node.closest(`.memberdecls`).remove()
        })
    }

    static addMemSeparator(node) {
        let td = document.createElement("td")
        td.setAttribute("colspan", "2")
        td.setAttribute("class", "memSeparator")
        td.textContent = '\xa0'

        let tr = document.createElement("tr");
        tr.setAttribute("class", "separator");
        tr.appendChild(td)

        node.appendChild(td)
    }
    static addMemDecl(name) {
        let h2 = document.createElement("h2")
        h2.setAttribute("class", "groupheader")
        h2.textContent = name

        this.addPermalinkTo(h2)

        let td = document.createElement("td")
        td.setAttribute("colspan", "2")
        td.appendChild(h2)

        let tr = document.createElement("tr");
        tr.setAttribute("class", "heading");
        tr.appendChild(td)

        let tbody = document.createElement("tbody");
        tbody.appendChild(tr)

        let table = document.createElement("table");
        table.setAttribute("class", "memberdecls");
        table.appendChild(tbody)

        let contents = document.getElementsByClassName("contents")[0];

        let nodes = contents.querySelectorAll(`[class="memberdecls"]`);
        let first = nodes[0];
        let last = nodes[nodes.length - 1];
        contents.insertBefore(table, last.nextSibling);

        return table;
    }

    static getMemDecl(name) {
        let anchor = document.querySelector(`.memberdecls a[name="${name}"]`)
        if (!anchor)
            return null
        return anchor.closest(`table.memberdecls`)
    }

    static GetGroupCategory(class_name) {
        // Parse custom group category
        let arr = class_name.split("____")
        if (arr.length > 1) {
            return arr[arr.length - 1]
        }

        // Get default group category
        // arr = class_name.split("__")
        // if (arr.length > 1) {
        //     return arr[arr.length - 1]
        // }

        return "Uncategorized"
    }

    static getValidID(text) {
        return text.replace(/\W/g,'_');
    }
}

window.DoxygenAwesomePermanentLink = DoxygenAwesomePermanentLink;
