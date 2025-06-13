# 入出力データ
``` mermaid
flowchart LR
    A[距離センサ] --> B(10msタイマー割込)
    B --> C{メイン}
    C --> D[ブザー]
```