
# ��ԑJ��
```mermaid
stateDiagram-v2
    [*] --> DoorClosed

    state DoorClosed {
        �h�A����
    }

    state DoorOpen {
        state OpenWait {
            �J������1������
        }
        state OpenAlarm {
            �J�����o�i�u�U�[�j
        }
        OpenWait --> OpenAlarm : �J������1���ȏ�
    }

    DoorClosed --> DoorOpen : �h�A�J�����o
    DoorOpen --> DoorClosed : �h�A�������o
```