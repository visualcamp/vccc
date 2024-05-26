class HeaderLink {
    static init() {
        $(function() {
            $(document).ready(function() {
                HeaderLink.updateMemberDecls()
            })
        })
    }

    static updateMemberDecls() {
        document.querySelectorAll(".groupheader").forEach((node, node_index) => {
            let has_child = node.querySelector(".anchor") != null;
            if (!has_child) {
                window.AddPermalink.addPermalinkTo(node, node_index)
            }
        })

        // Unnamed group
        document.querySelectorAll(".memberdecls .groupHeader").forEach((node, node_index) => {
            let has_child = node.querySelector(".anchor") != null;
            if (!has_child) {
                let heading = document.createElement("h3")
                heading.appendChild(node.firstChild)
                heading.classList.add("groupheader")

                let heading_id = window.AddPermalink.getValidID(heading.textContent.replaceAll(/[()]/g, ""))
                window.AddPermalink.addPermalinkTo(heading, heading_id)

                node.appendChild(heading)
            }
        })
    }
}
