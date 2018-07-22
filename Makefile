BUNDLE = pepperdistortion.lv2
INSTALL_DIR = /usr/local/lib/lv2


$(BUNDLE): manifest.ttl pepperdistortion.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp manifest.ttl pepperdistortion.ttl pepperdistortion.so $(BUNDLE)

pepperdistortion.so: pepperdistortion.cpp
	g++ -g -shared -fPIC -DPIC pepperdistortion.cpp `pkg-config --cflags --libs lv2-plugin` -o pepperdistortion.so

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)
	cp manifest.ttl $(INSTALL_DIR)/pepperdistortion.lv2/manifest.ttl
	cp pepperdistortion.ttl $(INSTALL_DIR)/pepperdistortion.lv2/pepperdistortion.ttl

uninstall:
	rm -rf $(INSTALL_DIR)/$(BUNDLE)

clean:
	rm -rf $(BUNDLE) pepperdistortion.so
