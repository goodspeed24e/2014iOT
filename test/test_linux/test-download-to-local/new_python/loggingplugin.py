#!/usr/bin/python

import os
from logging.handlers import RotatingFileHandler
from logging.handlers import BufferingHandler
import gzip

class CompressedRotatingFileHandler(RotatingFileHandler):
	"""Class for the file handler of logging."""

	def doRollover(self):
		"""
		This function will be called when the file handler need to do roll over.
		@param self The object pointer.
		"""
		if self.stream:
			self.stream.close()
			self.stream = None

		if self.backupCount > 0:
			old_gz_name = self.baseFilename + "." + str(self.backupCount) + ".gz"
			if os.path.exists(old_gz_name):
				os.remove(old_gz_name)

			for i in range(self.backupCount-1, 1, -1):
				old_gz_name = self.baseFilename + "." + str(i) + ".gz"
				new_gz_name = self.baseFilename + "." + str(i+1) + ".gz"
				if os.path.exists(old_gz_name):
					with gzip.open(old_gz_name, 'rb') as decomp_log:
						with gzip.open(new_gz_name, 'wb') as comp_log:
							comp_log.writelines(decomp_log)

			old_log_name = self.baseFilename + "." + str(1)
			new_gz_name = self.baseFilename + "." + str(2) + ".gz"
			if os.path.exists(old_log_name):
				with open(old_log_name) as log:
					with gzip.open(new_gz_name, 'wb') as comp_log:
						comp_log.writelines(log)
				os.remove(old_log_name)


			new_log_name = self.baseFilename + "." + str(1)
			os.rename(self.baseFilename, new_log_name)

		self.mode = 'w'
		self.stream = self._open()

class BufferingSMTPHandler(BufferingHandler):
	"""Class to buffer data then send email for logging."""

	def __init__(self, capacity, mailhost, fromaddr, toaddrs, subject, 
			is_ssl=True, credentials=None, secure=None):
		"""
		The constructor. (You can see the BufferingHandler and SMTPHandler of logging)
		@param self The object pointer.
		@param capacity The max capacity of buffer.
		@param mailhost The SMTP server (can specify port).
		@param fromaddr The from address of email.
		@param toaddrs The to address of email.
		@param subject The subject of email.
		@param is_ssl Use SSL or not.
		@param credentials The username and password.
		@param secure The keyfile and certificate file. 
		"""
		BufferingHandler.__init__(self, capacity)

		if isinstance(mailhost, tuple):
			self.mailhost, self.mailport = mailhost
		else:
			self.mailhost, self.mailport = mailhost, None
		if isinstance(credentials, tuple):
			self.username, self.password = credentials
		else:
			self.username = None
		self.fromaddr = fromaddr
		if isinstance(toaddrs, basestring):
			toaddrs = [toaddrs]
		self.toaddrs = toaddrs
		self.subject = subject
		self.secure = secure
		self.is_ssl = is_ssl

	def flush(self):
		"""
		This function will be called when the buffer need to be flushed.
		@param self The object pointer.
		"""
		if len(self.buffer) > 0:
			try:
				import smtplib
				from email.utils import formatdate
				port = self.mailport

				if self.is_ssl:
					if not port:
						port = smtplib.SMTP_SSL_PORT

					if self.secure is None:
						smtp = smtplib.SMTP_SSL(self.mailhost, port)
					else:
						smtp = smtplib.SMTP_SSL(self.mailhost, port, None, *self.secure)
				else:
					if not port:
						port = smtplib.SMTP_PORT
					smtp = smtplib.SMTP(self.mailhost, port)

				msg = "From: %s\r\nTo: %s\r\nSubject: %s\r\nDate: %s\r\n\r\n" % (
						self.fromaddr,
						",".join(self.toaddrs),
						self.subject,
						formatdate())

				for record in self.buffer:
					temp_text = self.format(record)
					msg += (temp_text + '\r\n')

				if self.username:
					if self.secure is not None and self.is_ssl == False:
						smtp.ehlo()
						smtp.starttls(*self.secure)
						smtp.ehlo()
					smtp.login(self.username, self.password)
				smtp.sendmail(self.fromaddr, self.toaddrs, msg)
				smtp.quit()
			except (KeyboardInterrupt, SystemExit):
				raise
			except:
				for record in self.buffer:
					self.handleError(record)
			finally:
				# If some errors occur, we still clear the buffer.
				self.buffer = []

			# We clear the buffer when everything is right.
			#self.buffer = []

