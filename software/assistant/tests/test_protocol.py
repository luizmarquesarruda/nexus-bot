import pytest

from nexus_assistant.protocol import ProtocolError, drive_frame, parse_response, stop_frame


def test_drive_frame_contains_short_ttl_and_newline():
    frame = drive_frame(7, 0.25, -0.5, 200)
    assert frame.endswith(b"\\n")
    assert b'"ttl_ms":200' in frame
    assert b'"seq":7' in frame


@pytest.mark.parametrize("left,right", [(1.1, 0), (-1.1, 0), (True, 0), (0, float("nan"))])
def test_drive_rejects_unsafe_speed(left, right):
    with pytest.raises(ProtocolError):
        drive_frame(1, left, right)


def test_drive_rejects_long_ttl():
    with pytest.raises(ProtocolError):
        drive_frame(1, 0, 0, 351)


def test_stop_frame_is_encoded():
    assert b'"name":"stop"' in stop_frame(2)


def test_parse_response_checks_version():
    assert parse_response(b'{"v":1,"type":"ack"}') ["type"] == "ack"
    with pytest.raises(ProtocolError):
        parse_response(b'{"v":2,"type":"ack"}')
