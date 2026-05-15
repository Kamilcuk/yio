import pytest
from yiofstring import preprocess, FStringError


def test_basic_fstring():
    assert preprocess('f"hello {x}"') == '"hello {}", x'


def test_fstring_with_format():
    assert preprocess('f"hello {x:02d}"') == '"hello {:02d}", x'


def test_fstring_with_equal():
    assert preprocess('f"hello {x=}"') == '"hello x={}", x'


def test_fstring_with_equal_format():
    assert preprocess('f"hello {x=:02d}"') == '"hello x={:02d}", x'


def test_dynamic_format():
    assert preprocess('f"{x:{width}.{prec}f}"') == '"{:{}.{}f}", x, width, prec'


def test_escaped_braces():
    assert preprocess('f"{{hello}} {x}"') == '"{{hello}} {}", x'


def test_merge_strings():
    assert preprocess('f"a" "b"') == '"ab"'
    assert preprocess('f"a" f"b"') == '"ab"'
    assert preprocess('f"a {x}" "b"') == '"a {}b", x'
    assert preprocess('"a" f"b {x}"') == '"ab {}", x'


def test_bracket_balancing():
    assert preprocess('f"{(1 + 2) * 3}"') == '"{}", (1 + 2) * 3'
    assert preprocess('f"{a[i]}"') == '"{}", a[i]'


def test_mismatched_brackets_error():
    # preprocess will call sys.exit(1) on FStringError
    with pytest.raises(SystemExit):
        preprocess('f"{(1 + 2}"')


def test_complex_ternary_with_brackets():
    assert preprocess('f"hello {(x + a ? 1 : 2):02d}"') == '"hello {:02d}", (x + a ? 1 : 2)'


def test_nested_quotes():
    # In C string literal: f"this is {\"nested\"}"
    assert preprocess(r'f"this is {\"nested\"}"') == '"this is {}", "nested"'


def test_strings_in_expression():
    # f"{(strcmp(s, \"}\") == 0)}"
    assert preprocess(r'f"{(strcmp(s, \"}\") == 0)}"') == '"{}", (strcmp(s, "}") == 0)'


def test_unterminated_string_error():
    with pytest.raises(SystemExit):
        preprocess('f"hello {x')


def test_line_directive_multiline_merge():
    # Multiline merge should have #line
    result = preprocess('f"a"\nf"b"')
    assert "#line" in result
    assert "#line 2" in result


def test_single_line_no_line_directive():
    # Single line should not have #line
    assert "#line" not in preprocess('f"{x}"')
    assert "#line" not in preprocess('f"{x}"\n')
