import keyring
import getpass

keyring.set_keyring(keyring.backend.CryptedFileKeyring())
#print keyring.backend.CryptedFileKeyring.filename # /.local/share/python_keyring/crypted_pass.cfg

print 'Please enter following passwords for the downloader.\n'

password = getpass.getpass('Please enter the password for the log mail: ')
keyring.set_password('streaming_pb_downloader', 'log mail', password)
print password

password = getpass.getpass('Please enter the password for the playback server (downloader): ')
keyring.set_password('streaming_pb_downloader', 'playback server', password)

password = getpass.getpass('Please enter the password for the alarm server: ')
keyring.set_password('streaming_pb_downloader', 'alarm server', password)

password = getpass.getpass('Please enter the password for the ApolloDB database of MySQL: ')
keyring.set_password('streaming_pb_downloader', 'ApolloDB', password)

