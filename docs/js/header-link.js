class HeaderLink {
    static init() {
        $(function() {
            $(document).ready(function() {
                HeaderLink.updateMemberDecls();
            })
        })
    }

    static updateMemberDecls() {
        document.querySelectorAll(".memberdecls .groupheader").forEach((node, node_index) => {
            let has_child = node.querySelector(".anchor") != null;
            if (!has_child) {
                window.AddPermalink.addPermalinkTo(node, node_index)
            }
        })
    }
}
