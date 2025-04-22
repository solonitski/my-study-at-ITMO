package markup;

public class Text implements inParagraph {
    private String text;

    public Text (String input) {
        text = input;
    }

    @Override
    public void toMarkdown(StringBuilder output) {
        output.append(text);
    }

    @Override
    public void toHtml(StringBuilder output) {
        output.append(text);
    }

    @Override
    public void toTex(StringBuilder output) {
        output.append(text);
    }
}
