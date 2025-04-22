package markup;

import java.util.List;

public abstract class AbstractCon implements inParagraph {
    protected List <inParagraph> text;
    protected String tagFirst, tagSecond;

    public AbstractCon(List <inParagraph> input) {
        text = input;
    }

    @Override
    public void toMarkdown(StringBuilder output) {

        output.append(tagFirst);
        for (inParagraph elements : text) {
            elements.toMarkdown(output);
        }
        output.append(tagSecond);
    }

    @Override
    public void toHtml(StringBuilder output) {
        output.append(tagFirst);
        for (inParagraph elements : text) {
            elements.toHtml(output);
        }
        output.append(tagSecond);
    }

    @Override
    public void toTex(StringBuilder output) {
        output.append(tagFirst);
        for (inParagraph elements : text) {
            elements.toTex(output);
        }
        output.append(tagSecond);
    }
}
