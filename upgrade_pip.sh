source /opt/venv/bin/activate &&
python -m pip freeze > requirements.before-upgrade.txt &&
python -m pip install --upgrade pip setuptools wheel &&
python -m pip list --outdated --format=json --exclude-editable \
  | python -c 'import sys,json; print("\n".join(x["name"] for x in json.load(sys.stdin)))' \
  | xargs -r python -m pip install --upgrade &&
python -m pip check

