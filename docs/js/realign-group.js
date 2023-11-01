
class RealignGroup {

    static init() {
        $(function() {
            $(document).ready(function() {
                RealignGroup.MoveGroups()
            })
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
                    memdecl = this.addMemDecl(category_id);

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
}
