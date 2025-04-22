package markup;

import java.util.List;

public class Emphasis extends AbstractCon {
    public Emphasis(List <inParagraph> input) {
        super(input);
    }

    @Override
    public void toMarkdown(StringBuilder out) {
        super.tagFirst = "*";
        super.tagSecond = "*";
        super.toMarkdown(out);
    }

    @Override
    public void toHtml(StringBuilder out) {
        super.tagFirst = "<em>";
        super.tagSecond = "</em>";
        super.toHtml(out);
    }

    @Override
    public void toTex(StringBuilder out) {
        super.tagFirst = "\\emph{";
        super.tagSecond = "}";
        super.toTex(out);
    }
}