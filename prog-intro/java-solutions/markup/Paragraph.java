package markup;

import java.util.List;

public class Paragraph implements inParagraph {
    List <inParagraph> text;

    public Paragraph(List <inParagraph> input) {
        text = input;
    }

    @Override
    public void toMarkdown(StringBuilder output) {
        for (inParagraph elements : text) {
            elements.toMarkdown(output);
        }
    }

    @Override
    public void toHtml(StringBuilder output) {
        for (inParagraph elements : text) {
            elements.toHtml(output);
        }
    }

    @Override
    public void toTex(StringBuilder output) {
        for (inParagraph elements : text) {
            elements.toTex(output);
        }
    }
}
