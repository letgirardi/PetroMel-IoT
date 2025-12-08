# 1. Base Image and clear ENTRYPOINT (from previous fix)
FROM nodered/node-red:latest
ENTRYPOINT []

# 2. Setup environment and working directory
USER root 
WORKDIR /data

# 3. Copy files needed for node installation (package.json and flow files)
COPY package.json package-lock.json ./
COPY node-red /data

# 4. Install Node-RED Nodes (The Palette)
RUN npm install --unsafe-perm --no-progress

# 5. Set final configuration and startup command
USER node-red 
EXPOSE 1880

# Run the Node-RED instance
CMD ["/usr/bin/env", "node-red", "--userDir", "/data"]