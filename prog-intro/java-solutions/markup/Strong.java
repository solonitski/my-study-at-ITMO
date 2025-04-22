package markup;

import java.util.List;

public class Strong extends AbstractCon {
    public Strong(List <inParagraph> input) {
        super(input);
    }

    @Override
    public void toMarkdown(StringBuilder out) {
        super.tagFirst = "__";
        super.tagSecond = "__";
        super.toMarkdown(out);
    }

    @Override
    public void toHtml(StringBuilder out) {
        super.tagFirst = "<strong>";
        super.tagSecond = "</strong>";
        super.toHtml(out);
    }

    @Override
    public void toTex(StringBuilder out) {
        super.tagFirst = "\\textbf{";
        super.tagSecond = "}";
        super.toTex(out);
    }
}
