
class RealignGroup {

    static kUncategorized = "Uncategorized"

    static init() {
        $(function() {
            $(document).ready(function() {
                RealignGroup.MoveGroups()
            })
        })
    }

    // Move items in 'Groups' header under another header
    static MoveGroups() {
        document.querySelectorAll(`.memberdecls [id="groups"]`).forEach((node) => {
            node.closest(`tbody`).querySelectorAll(`tr[class*="memitem"]`).forEach((node) => {
                let category = this.GetGroupCategory(node.getAttribute("class"));
                this.MoveOneGroup(node, category.map((item) => this.GetCategoryPair(item)))
            })

        })

        // Remove 'Groups' header
        let group = document.querySelector(`.memberdecls [id="groups"]`)
        if (group) {
            group.closest(`.memberdecls`).remove()
        }

        let uncategorized = this.GetMemberDecls(this.kUncategorized)
        if (uncategorized) {
            this.MoveDeclToLast(uncategorized)
        }

        let typedef = this.GetMemberDecls("typedef-members")
        if (typedef) {
            this.MoveDeclToLast(typedef)
        }

    }

    static GetCategoryPair(name) {
        switch (name) {
            case "func":
            case "function":
            case "Function":
            case "functions":
            case "Functions":
                return ["func-members", "Functions"]

            case "class":
            case "Class":
            case "classes":
            case "Classes":
                return ["nested-classes", "Classes"]

            case "alias":
            case "typedef":
            case "Typedef":
            case "Typedefs":
            case "using":
                return ["typedef-members", "Typedefs"]

            case "var":
            case "variable":
                return ["var-members", "Variables"]

            case "def":
            case "define":
            case "macro":
                return ["define-members", "Variables"]

            default:
                return [window.AddPermalink.getValidID(name), name]
        }
    }

    static MoveOneGroup(node, categories) {
        let memdecl = this.getMemDecl(categories[0][0])
        if (!memdecl) {
            memdecl = this.addMemDecl(categories[0][1], categories[0][0], "h2");
        }

        let tbody = memdecl.querySelector(`tbody`)
        let pos = tbody.lastChild.nextSibling

        if (categories.length > 1) {
            let nested = this.getMemDecl(categories[1][0])
            if (!nested) {
                let header = this.createHeader(categories[1][1], categories[1][0], "h3")
                tbody.appendChild(header)
                pos = tbody.lastChild.nextSibling
            } else {
                let heading = nested.querySelector(`a[name="${categories[1][0]}"]`).closest("tr.heading")
                // TODO: Insert back
                pos = heading.nextSibling
            }
        }

        let next = node.nextSibling.nextSibling
        tbody.insertBefore(node, pos)

        // Copy memitem, sep, etc
        while (next) {
            if (next.getAttribute("class").includes("memitem"))
                break

            let next2 = next.nextSibling.nextSibling
            next = tbody.insertBefore(next, pos)
            next = next2
        }
    }

    static GetMemberDecls(name) {
        let header = document.querySelector(`.memberdecls [id="${name}"]`)
        if (header) {
            return  header.closest(`.memberdecls`)
        }
        return null
    }

    static MoveDeclToLast(node) {
        let arr = document.querySelectorAll(`.memberdecls`)
        arr[arr.length - 1].parentNode.insertBefore(node, arr[arr.length - 1].nextSibling)
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

    static createHeader(name, id, heading) {
        let h2 = document.createElement(heading)
        h2.setAttribute("class", "groupheader")
        h2.textContent = name

        window.AddPermalink.addPermalinkTo(h2, id)

        let td = document.createElement("td")
        td.setAttribute("colspan", "2")
        td.appendChild(h2)

        let tr = document.createElement("tr");
        tr.setAttribute("class", "heading");
        tr.appendChild(td)

        return tr
    }

    static addMemDecl(name, id, heading) {
        let tr = this.createHeader(name, id, heading)

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
            let categories = arr.slice(1)
            let parsed_categories = []
            categories.forEach((name) => {
                // Parse space
                let words = name.split("__")
                for (let i = 0; i < words.length; ++i) {
                    // Parse upper case
                    if (words[i].includes("_")) {
                        words[i] =  words[i].charAt(1).toUpperCase() + words[i].slice(2)
                    }
                }
                parsed_categories.push(words.join(" "))
            })

            return parsed_categories
        }

        return [this.kUncategorized]
    }
}
