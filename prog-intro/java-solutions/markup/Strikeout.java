package markup;

import java.util.List;

public class Strikeout extends AbstractCon {
    public Strikeout(List <inParagraph> input) {
        super(input);
    }

    @Override
    public void toMarkdown(StringBuilder out) {
        super.tagFirst = "~";
        super.tagSecond = "~";
        super.toMarkdown(out);
    }

    @Override
    public void toHtml(StringBuilder out) {
        super.tagFirst = "<s>";
        super.tagSecond = "</s>";
        super.toHtml(out);
    }

    @Override
    public void toTex(StringBuilder out) {
        super.tagFirst = "\\textst{";
        super.tagSecond = "}";
        super.toTex(out);
    }
}
