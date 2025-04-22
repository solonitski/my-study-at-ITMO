package markup;

public interface inParagraph {
    void toMarkdown(StringBuilder output);
    void toHtml(StringBuilder output);
    void toTex(StringBuilder output);
}
