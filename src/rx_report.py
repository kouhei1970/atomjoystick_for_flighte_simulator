import hid
import time

def list_hid_devices():
    """接続されているHIDデバイスの情報を列挙する"""
    print("=== 接続されているHIDデバイス一覧 ===")
    for d in hid.enumerate():
        info = {
            'vendor_id': hex(d['vendor_id']),
            'product_id': hex(d['product_id']),
            'manufacturer': d.get('manufacturer_string'),
            'product': d.get('product_string')
        }
        print(info)
    print("====================================\n")

def main():
    # まずは全 HID デバイスの一覧を表示し、目的のデバイスのIDを確認します
    list_hid_devices()
    
    # ※以下の VENDOR_ID と PRODUCT_ID はご利用のゲームパッド（ESP32‑S3 で HID ゲームパッドとして動作している場合）の値に合わせて変更してください
    VENDOR_ID = 0x303a   # 例: 0x1209 (実際の値に合わせてください)
    PRODUCT_ID = 0x1001  # 例: 0xABCD (実際の値に合わせてください)
    
    try:
        device = hid.Device(VENDOR_ID, PRODUCT_ID)
        print("デバイスをオープンしました:", device.manufacturer, device.product)
        
        # 非ブロッキングモードに設定
        device.nonblocking = True
        
        print("HIDレポートの受信を開始します (Ctrl+C で終了)...")
        while True:
            data = device.read(5)  # 1回の読み込みで最大 64 バイト取得
            if data:
                # 受信した生データ（バイト列）を表示
                print(f"{data[0]:03d}, {data[1]:03d}, {data[2]:03d}, {data[3]:03d}, {data[4]:03d}") # , {data[5]:03d}, {data[6]:03d}, {data[7]:03d}, {data[8]:03d}")
            time.sleep(0.01)  # 過剰なCPU使用を避けるため少し待機
    except Exception as e:
        print("エラー:", e)
    finally:
        try:
            device.close()
        except Exception:
            pass

if __name__ == "__main__":
    main()
