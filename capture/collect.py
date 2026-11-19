import os, time, requests

URL = "http://192.168.x.xx/capture"  # Đổi thành IP in trên Serial Monitor sau khi chạy data_collector
LABELS = {"1": "electronic_waste", "2": "other_waste", "3": "empty_tray"}

for folder in LABELS.values():
    os.makedirs(f"dataset/{folder}", exist_ok=True)

print("1: Rác linh kiện | 2: Rác khác | 3: Khay trống | q: Thoát")
while True:
    key = input("Chọn nhãn chụp: ").strip()
    if key == "q":
        break
    if key in LABELS:
        r = requests.get(URL, timeout=5)
        if r.status_code == 200:
            fname = f"dataset/{LABELS[key]}/{int(time.time()*1000)}.jpg"
            with open(fname, "wb") as f:
                f.write(r.content)
            count = len(os.listdir(f"dataset/{LABELS[key]}"))
            print(f"Đã lưu {fname} (Tổng {LABELS[key]}: {count})")