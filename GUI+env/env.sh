rm -rf venv-up-all
python3.11 -m venv venv-up-all

source venv-up-all/bin/activate
python -m pip install --upgrade pip
pip install --force-reinstall -r requirements.txt