FROM fedora:rawhide

# Install any needed packages
RUN dnf install -y openssh-server gdb

RUN /usr/libexec/openssh/sshd-keygen rsa

RUN mkdir -m 700 /root/.ssh

ADD authorized_keys /root/.ssh

EXPOSE 22

CMD ["/usr/sbin/sshd", "-Dddd"]
